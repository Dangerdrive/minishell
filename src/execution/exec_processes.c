#include "../../includes/minishell.h"

void	restore_fds(int original_fds[2]);
int		wait_for_children(int children_pid[1024]);

static void	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
}

static int	*init_children_pid(t_global *data)
{
	int		*children_pid;
	size_t	size;

	size = sizeof(int) * (pipecount(data) + 2);
	children_pid = malloc(size);
	if (!children_pid)
		return (NULL);
	ft_bzero(children_pid, size);
	return (children_pid);
}

int	execute_forked_builtin(char **args, int idx, t_global *data)
{
	int	exit_status;

	exit_status = exec_builtin(args, hashsize(data->hashtable[idx]), data);
	ft_strarr_free(args, ft_strarr_len(args));
	rl_clear_history();
	data->ret = exit_status;
	exit(exit_status);
}

static void	execute_forked_command(t_global *data, int idx)
{
	char	**args;

	close_extra_fds();
	args = hash_to_args(data->hashtable[idx]);
	if (is_builtin(args[0]))
		execute_forked_builtin(args, idx, data);
	else
	{
		exec_nonbuiltin(args, data);
	}
}

int	exec_processes(t_global *data)
{
	int	*children_pid;
	int	i;
	int j;

	i = 0;
	j = 0;
	save_original_fds(data->original_fds);
	while (data->hashtable[j])
	{
		while (data->hashtable[j]->redir[i])
		{
			if (ft_strncmp(data->hashtable[0]->redir[i], "<<", 2) == 0)
				write_in_heredoc(data, i, j, &data->hashtable[j]->redir[i][2]);
			i++;
		}
		j++;
	}
	children_pid = init_children_pid(data);
	while (data->hashtable[i])
	{
		handle_pipe(data->original_fds[OUT], data,
			data->hashtable[i], data->hashtable);
		children_pid[i] = fork();
		define_exec_signals(children_pid[i]);
		if (children_pid[i] == -1)
			ft_dprintf(2, "minishell: %s: %s\n", "fork", strerror(errno));
		else if (children_pid[i] == 0)
		{
			handle_redirects_for_pipes(&data->hashtable[i]->redir);
			execute_forked_command(data, i);
		}
		i++;
	}
	restore_fds(data->original_fds);
	i = wait_for_children(children_pid);
	free(children_pid);
	return (i);
}
