#include "../../includes/minishell.h"

int	wait_for_child(int child_pid, int is_last_child)
{
	int	status;

	if (child_pid == -1)
		return (EXIT_FAILURE);
	if (waitpid(child_pid, &status, 0) == -1)
		ft_dprintf(2, "minishell: waitpid: %s\n", strerror(errno));
	// if (WIFSIGNALED(status))
	// 	return (handle_signal_interrupt(status, is_last_child)); //handle signals
	(void)is_last_child;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

int	wait_for_children(int children_pid[1024])
{
	int	i;
	int	exit_status;
	int	is_last_child;

	i = 0;
	exit_status = 0;
	while (children_pid[i] != 0)
	{
		is_last_child = children_pid[i + 1] == 0;
		exit_status = wait_for_child(children_pid[i], is_last_child);
		i++;
	}
	close_extra_fds();
	free(children_pid);
	return (exit_status);
}

int	execute_forked_builtin(char **args, int idx, t_global *data)
{
	int	exit_status;

	exit_status = exec_builtin(args, hashsize(data->hashtable[idx]), data);
	ft_strarr_free(args, ft_strarr_len(args));
	rl_clear_history();
	exit(exit_status);
}

static void	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
}

void	quit_child(char **args)
{
	rl_clear_history();
	ft_strarr_free(args, ft_strarr_len(args));
	close_all_fds();
	exit(EXIT_FAILURE);
}

// static void	handle_redirect(char *command, char **commands, t_env **minienv)
// static void	handle_redirects(t_global *data, char **redirects, int pid)
// {
// 	// char	redirect;
// 	int	i;
// 	//redirect = get_next_redirect(command);

// 	i = 0;
// 	while (redirects[i])
// 	{
// 	if (ft_strncmp(redirects[i], "< ", 2) == 0)
// 		{
// 			if (redirect_input(&redirects[i][2]) == 0)
// 				quit_child(redirects);
// 		}
// 		if (redirects[i][0] == '>')
// 		{
// 			if (redirect_output(&redirects[i][2]) == 0)
// 				quit_child(redirects);
// 		}
// 		if (ft_strncmp(redirects[i], "<<", 2) == 0)
// 			redirect_heredoc(data, pid, i, &redirects[i][2]);
// 		i++;
// 	}
// }

// static void	execute_forked_command(char *command, char **commands,
// 		t_env **minienv)
static void	execute_forked_command(t_global *data, int idx)
{
	char	**args;

	close_extra_fds();
	args = hash_to_args(data->hashtable[idx]);

	if (is_builtin(args[0]))
		execute_forked_builtin(args, idx, data);
	else
		exec_nonbuiltin(args, data);
}

static void	restore_original_fdsX(int original_fds[2])
{
	redirect_fd(original_fds[IN], STDIN_FILENO);
	redirect_fd(original_fds[OUT], STDOUT_FILENO);
}

// void	handle_pipe(int original_fd_out, char *current_command, char **commands)
void	handle_pipe(int original_fd_out, t_global *data, t_tkn *current_node, t_tkn **hashtable)
{
	t_bool		is_first_node;
	t_bool		has_next_node;
	t_tkn		*last_node;
	static int	pipe_fds[2];

	last_node = hashtable[pipecount(data)];
	is_first_node = (current_node == hashtable[0]);
	has_next_node = (current_node != last_node);
	if (!is_first_node)
		redirect_fd(pipe_fds[IN], STDIN_FILENO);
	if (has_next_node)
	{
		if (pipe(pipe_fds) == -1)
			ft_dprintf(2, "minishell: %s: %s\n", "pipe", strerror(errno));
//			print_perror_msg("pipe", current_command);
		redirect_fd(pipe_fds[OUT], STDOUT_FILENO);
	}
	else
		redirect_fd(original_fd_out, STDOUT_FILENO);
}

int	*init_children_pid(t_global *data)
{
	int		*children_pid;
	size_t	size;

//	size = sizeof(int) * (arr_len(commands) + 1);
	size = sizeof(int) * (pipecount(data) + 2);
	children_pid = malloc(size);
	if (!children_pid)
		return (NULL);
	ft_bzero(children_pid, size);
	return (children_pid);
}

//int	execute_multiple_commands(char **commands, t_env **minienv)
int	exec_processes(t_global *data)
{
	int	original_fds[2];
	int	*children_pid;
	int	i;

	save_original_fds(original_fds);
	children_pid = init_children_pid(data);
	i = 0;
	while (data->hashtable[i])
	{
		handle_pipe(original_fds[OUT],data, data->hashtable[i], data->hashtable);
		children_pid[i] = fork();
		//printf("children_pid[%d] = %d\n", i, children_pid[i]);
		//define_execute_signals(children_pid[i]); //handle signals
		if (children_pid[i] == -1)
			ft_dprintf(2, "minishell: %s: %s\n", "fork", strerror(errno));
		else if (children_pid[i] == 0)
		{
			//free(children_pid);
			//handle_redirects(data, data->hashtable[i]->redir, children_pid[i]);
			handle_redirects(data, original_fds); //MUDEI AQUI
			execute_forked_command(data, i);
		}
		i++;
	}
	restore_original_fdsX(original_fds);
	return (wait_for_children(children_pid));
}
