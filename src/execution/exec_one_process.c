#include "../../includes/minishell.h"

void	exec_nonbuiltin(char **args, t_global *data)
{
	char	*cmd;

	// if (args[0][0])
	// 	external_exit(EXIT_SUCCESS);
	// if (is_folder(args[0]))
	// 	external_exit(NOT_EXECUTABLE);
	cmd = get_cmd(args[0], data);
	//printf("fd0: %d | fd1: %d\n", data->original_fds[0], data->original_fds[1]);
	if (cmd)
	{
		//printf("ENTROU AQUI %s\n", args[0]);
		if (execve(cmd, args, data->env) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
				args[0], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", args[0]);
		rl_clear_history();
		ft_strarr_free(args, ft_strarr_len(args));
		close_all_fds();
		exit(CMD_NOT_FOUND);
	}
}

static int	handle_redirects_one_cmd(t_global *data, int ori_fds[2])
{
	int		i;

	i = 0;
	ori_fds[IN] = -1;
	ori_fds[OUT] = -1;
	while (data->hashtable[0]->redir[i])
	{
		if (ft_strncmp(data->hashtable[0]->redir[i], "< ", 2) == 0)
		{
			if (!handle_input_redirect(data->hashtable[0]->redir[i], ori_fds))
				return (0);
		}
		if (data->hashtable[0]->redir[i][0] == '>')
		{
			if (!handle_output_redirect(data->hashtable[0]->redir[i], ori_fds))
				return (0);
		}
		if (ft_strncmp(data->hashtable[0]->redir[i], "<<", 2) == 0)
		{
			save_original_fd_in(ori_fds);
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
		exec_nonbuiltin(args, data);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			ft_dprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		else if (WIFSIGNALED(status))
			ret = handle_signal_interrupt(status, TRUE);
		else if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
	}
	return (ret);
}

int	exec_nonbuiltin_onfork(t_global *data, char **args)
{
	int		pid;
	int		ret;

	ret = 1;
	pid = fork();
	if (handle_redirects_one_cmd(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (EXIT_FAILURE);
	}
	if (args && args[0] && !is_builtin(args[0]))
	{
		ret = exec_nonbuiltin_and_wait(data, args, pid);
		restore_original_fds(data->original_fds);
	}
	return (ret);
}

int	exec_one_process(t_global *data)
{
	int		ret;
	char	**args;

	ret = 1;
	args = NULL;
	// ADD VERIFICAÇÃO DE CONTENT PARA PASSAR AQUI SÓ EM CASO DO COMANDO TER APENAS REDIR.
	if (!data->hashtable[0]->content && handle_redirects(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (EXIT_FAILURE);
	}
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (args && args[0] && is_builtin(args[0]))
		ret = exec_builtin(args, hashsize(data->hashtable[0]), data);
	else if (args && args[0])
		ret = exec_nonbuiltin_onfork(data, args);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	restore_original_fds(data->original_fds);
	return (ret);
}
