#include "../../includes/minishell.h"


int	pipecount(t_global *data)
{
	int	result;

	result = 0;
	while (data->hashtable[result])
	{
		result++;
	}
	return (result - 1);
}
//-----------------------------------------------------------------------------|
// void	exec_command(t_global **data, int idx)
// {
// 	char	**args;
// 	char	*cmd;

// 	parse_redirections(data, &(*data)->hashtable[idx]);
// 	// if ((*data)->hashtable[idx]->content == NULL)
// 	// 	exit(EXIT_SUCCESS);
// 	args = hash_to_args((*data)->hashtable[idx]);
// 	if (is_builtin(args[0]))
// 		(*data)->ret = exec_builtin(args, hashsize((*data)->hashtable[idx]), *data);
// 	else
// 	{
// 		(*data)->ret = 127; //arrumar valor de retorno para comandos que nao existem
// 		cmd = get_cmd(args[0], *data);
// 		if (cmd)
// 			(*data)->ret = execve(cmd, args, (*data)->env); //consolidar env talvez
// 		//perror("minishell: execve");
// 	}
// 	ft_strarr_free(args, ft_strarr_len(args));
// 	//exit(EXIT_FAILURE);
// }

// void	create_pipes(int pipes[][2], int n)
// {
// 	int	i;

// 	i = 0;
// 	while (i < n)
// 	{
// 		if (pipe(pipes[i]) == -1)
// 		{
// 			perror("minishell: pipe:");
// 			exit(EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// }

// void	close_pipes(int pipes[][2], int n)
// {
// 	int	i;

// 	i = 0;
// 	while (i < n)
// 	{
// 		close(pipes[i][0]);
// 		close(pipes[i][1]);
// 		i++;
// 	}
// }

// void	fork_processes(t_global **data, int pipes[][2], int n)
// {
// 	int	i;

// 	i = 0;
// 	while (i <= n)
// 	{
// 		(*data)->pid = fork();
// 		if ((*data)->pid == -1)
// 		{
// 			perror("minishell: fork");
// 			exit(EXIT_FAILURE);
// 		}
// 		else if ((*data)->pid == 0)
// 		{
// 			if (i > 0)
// 				dup2(pipes[i - 1][0], STDIN_FILENO);
// 			if (i < n)
// 				dup2(pipes[i][1], STDOUT_FILENO);
// 			close_pipes(pipes, n);
// 			// if (parse_redirections((*data)->hashtable[i]) == 1) not included yet
// 			// 	return;
// 			if ((*data)->hashtable[i]->content == NULL)
// 				return ;
// 			exec_command(data, n); // TROQUEI O SEGUNDO ARG DE 'i' PARA 'n'
// 		}
// 		i++;
// 	}
// }
//-----------------------------------------------------------------------------|


// int	exec(t_global *data)
// {
// 	int	n;
// 	int	(*pipes)[2];

// 	n = pipecount(data);
// 	if (data->hashtable[0]->content == NULL && n == 0)
// 		return (0);
// 	pipes = malloc(n * sizeof(*pipes));
// 	if (pipes == NULL)
// 	{
// 		perror("minishell: pipes: malloc failed");
// 		return (-1);
// 	}
// 	create_pipes(pipes, n);
// 	fork_processes(data, pipes, n);
// 	close_pipes(pipes, n);
// 	n = 2;
// 	while (++n < 1024)
// 		close(n);
// 	while (wait(NULL) > 0)

// 	free(pipes);
// 	return (0);
// }

//-----------------------------------------------------------------------------|
// int exec(t_global **data)
// {
// 	int n;
// 	int(*pipes)[2];
// 	int status;
// 	pid_t pid;

// 	n = pipecount(*data);
// 	if ((*data)->hashtable[0]->content == NULL && n == 0)
// 		return (0);
// 	pipes = malloc(n * sizeof(*pipes));
// 	if (pipes == NULL)
// 	{
// 		perror("minishell: pipes: malloc failed");
// 		return (-1);
// 	}
// 	create_pipes(pipes, n);
// 	fork_processes(data, pipes, n);
// 	close_pipes(pipes, n);
// 	n = 2;
// 	while (++n < 1024)
// 		close(n);
// 	pid = waitpid(-1, &status, 0);
// 	while (pid != -1)
// 		pid = waitpid(-1, &status, 0);
// 	free(pipes);
// 	return (0);
// }
//-----------------------------------------------------------------------------|

// void	close_extra_fds(void)
// {
// 	int	last_open_fd;

// 	last_open_fd = open("/tmp/last_fd", O_RDWR | O_CREAT, 0666);
// 	if (last_open_fd == -1)
// 		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", "/tmp/last_fd", strerror(errno));
// 		// print_perror_msg("open", "/tmp/last_fd");
// 	while (last_open_fd > STDERR_FILENO)
// 	{
// 		close(last_open_fd);
// 		last_open_fd--;
// 	}
// }

// void	close_all_fds(void)
// {
// 	close_extra_fds();
// 	close(STDIN_FILENO);
// 	close(STDOUT_FILENO);
// 	close(STDERR_FILENO);
// }

void	exec_nonbuiltin(char **args, t_global *data)
{
	char	*cmd;

	cmd = get_cmd(args[0], data);
	if (cmd)
	{
		if (execve(cmd, args, data->env) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", args[0], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", args[0]);
		rl_clear_history();
		//freeenv?
		ft_strarr_free(args, ft_strarr_len(args));
		close_all_fds();	
		
		exit(127);//command not found.
	}
}

int	exec_one_nonbuiltin(t_global *data, char **args)
{
	pid_t	pid;
	int		ret;
	int		status;

	ret = 1;
	pid = fork();
	//define_execute_signals(child_pid); lidar com sinais

	if (pid == -1)
		perror("minishell: fork");
	else if (pid == 0)
	{
		exec_nonbuiltin(args, data);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
        	ft_dprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		//	("waitpid", ft_itoa(pid));
		// lidar com sinal
        // else if (WIFSIGNALED(status))
        //     ret = handle_signal_interrupt(status, TRUE);
        else if (WIFEXITED(status))
            ret = WEXITSTATUS(status);
    }
	return(ret); 
}
/*
int exec_processes(t_global *data)
{
	
}
*/

int	exec_one_process(t_global *data)
{
	int		ret;
	char	**args;

	ret = 1;
	args = NULL;
	if (handle_redirect(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (1);
	}
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (args && args[0] && is_builtin(args[0]))
		ret = exec_builtin(args, hashsize(data->hashtable[0]), data);
	else if (args && args[0] && !is_builtin(args[0]))
		ret = exec_one_nonbuiltin(data, args);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	restore_original_fds(data->original_fds);
	return (ret);
}

int	handle_execution(t_global *data)
{
	int		ret;
	char	**args;

	args = NULL;
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	ret = 1;
	if (pipecount(data) == 0)
		exec_one_process(data);
	// else if (pipecount(data) > 0)
	// 	exec_process(data);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	return (ret);
}





static void	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
}

static void	handle_redirect(char *command, char **commands, t_env **minienv)
{
	char	redirect;

	redirect = get_next_redirect(command);
	while (redirect)
	{
		if (redirect == '<')
		{
			if (redirect_input(command) == FAILED)
				quit_child(commands, minienv);
		}
		if (redirect == '>')
		{
			if (redirect_output(command) == FAILED)
				quit_child(commands, minienv);
		}
		if (redirect < 0)
			redirect_heredoc(command, redirect);
		redirect = get_next_redirect(command);
	}
}

// static void	execute_forked_command(char *command, char **commands,
// 		t_env **minienv)
static void	execute_forked_command(t_global *data, int idx)
{
	char	**args;

	close_extra_fds();
	args = hash_to_args(data->hashtable[idx]);

	if (is_builtin(args[0]))
		execute_forked_builtin(args, minienv);
	else
		execute_external(args, *minienv);
}

static void	restore_original_fds(int original_fds[2])
{
	redirect_fd(original_fds[IN], STDIN_FILENO);
	redirect_fd(original_fds[OUT], STDOUT_FILENO);
}

void	handle_pipe(int original_fd_out, char *current_command, char **commands)
{
	int			is_first_command;
	int			has_next_command;
	char		*last_command;
	static int	pipe_fds[2];

	last_command = commands[arr_len(commands) - 1];
	is_first_command = (current_command == commands[0]);
	has_next_command = (current_command != last_command);
	if (!is_first_command)
		redirect_fd(pipe_fds[IN], STDIN_FILENO);
	if (has_next_command)
	{
		if (pipe(pipe_fds) == -1)
			print_perror_msg("pipe", current_command);
		redirect_fd(pipe_fds[OUT], STDOUT_FILENO);
	}
	else
		redirect_fd(original_fd_out, STDOUT_FILENO);
}

int	execute_multiple_commands(char **commands, t_env **minienv)
int	exec_processes(t_global *data)
{
	int	original_fds[2];
	int	*children_pid;
	int	i;

	save_original_fds(original_fds);
	children_pid = init_children_pid(commands);
	i = 0;
	while (commands[i])
	{
		handle_pipe(original_fds[OUT], commands[i], commands);
		children_pid[i] = fork();
		//define_execute_signals(children_pid[i]); //handle signals
		if (children_pid[i] == -1)
			print_perror_msg("fork", commands[i]);
		else if (children_pid[i] == 0)
		{
			free(children_pid);
			handle_redirect();
			execute_forked_command(commands[i], commands, minienv);
		}
		i++;
	}
	restore_original_fds(original_fds);
	return (wait_for_children(children_pid));
}
