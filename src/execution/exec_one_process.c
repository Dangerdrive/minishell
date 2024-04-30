#include "../../includes/minishell.h"

int	exec_nonbuiltin(char **args, t_global *data)
{
	char	*cmd;

	if (!args[0][0] || !ft_strcmp(*args, " "))
		external_exit(EXIT_SUCCESS);
	cmd = get_cmd(args[0], data);
	if (cmd == NULL)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", args[0]);
		ft_strarr_free(args, ft_strarr_len(args));
		external_exit(CMD_NOT_FOUND);
	}
	if (is_folder(args[0]))
		external_exit(NOT_EXECUTABLE);
	if (cmd)
	{
		if (execve(cmd, args, data->env) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
				args[0], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	return (data->ret);
}

static int	handle_redirects_one_cmd(t_global *data, int (*ori_fds)[2])
{
	int		i;

	i = 0;
	(*ori_fds)[IN] = -1;
	(*ori_fds)[OUT] = -1;
	data->original_fds[IN] = -1;
	data->original_fds[OUT] = -1;
	while (data->hashtable[0]->redir[i])
	{
		if (ft_strncmp(data->hashtable[0]->redir[i], "< ", 2) == 0)
		{
			if (!handle_input_redirect(data->hashtable[0]->redir[i], data->original_fds))
				return (0);
		}
		if (data->hashtable[0]->redir[i][0] == '>')
		{
			if (!handle_output_redirect(data->hashtable[0]->redir[i], data->original_fds))
				return (0);
		}
		if (ft_strncmp(data->hashtable[0]->redir[i], "<<", 2) == 0)
		{
			save_original_fd_in(data->original_fds);
			redirect_heredoc(data, i, &data->hashtable[0]->redir[i][2]);
		}
		i++;
	}
	return (1);
}

int	exec_nonbuiltin_and_wait(t_global *data, char **args, int pid)
{
	int	status;
	int	ret;

	ret = 1;
	if (pid == -1)
		perror("minishell: fork");
	else if (pid == 0)
	{
		ret = exec_nonbuiltin(args, data);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
			ret = handle_signal_interrupt(status, TRUE);
		else if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
	}
	return (ret);
}

void	exec_nonbuiltin_onfork(t_global *data, char **args)
{
	int		pid;
	//int		ret;

	data->ret = 1;
	pid = fork();
	if (handle_redirects_one_cmd(data, &data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		external_exit(EXIT_FAILURE);
	}
	if (args && args[0] && !is_builtin(args[0]))
	{
		data->ret = exec_nonbuiltin_and_wait(data, args, pid);
		restore_original_fds(data->original_fds);
	}
	//return (ret);
	external_exit(EXIT_SUCCESS);
}

int	exec_one_process(t_global *data)
{
	int		ret;
	char	**args;

	ret = 1;
	args = NULL;
	// if (!data->hashtable[0]->content
	// 	&& handle_redirects(data, data->original_fds) == 0)
	// {
	// 	restore_original_fds(data->original_fds);
	// 	return (EXIT_FAILURE);
	// }
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (args && args[0] && is_builtin(args[0]))
		ret = exec_builtin(args, hashsize(data->hashtable[0]), data);
	else if (args && args[0])
		exec_nonbuiltin_onfork(data, args);
	else if (!data->hashtable[0]->content
		&& handle_redirects(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (EXIT_FAILURE);
	}
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	restore_original_fds(data->original_fds);
	return (ret);
}
