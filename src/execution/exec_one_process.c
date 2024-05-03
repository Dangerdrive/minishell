/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:58:48 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 22:04:50 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_in_heredoc(t_global *data, int heredoc_number,
			int hash_number, char *eof);

/**
 * Executes a non-builtin command found in the system's PATH.
 * 
 * @param[in] args Array of arguments for the command.
 * @param[in, out] data Global data structure including the environment.
 * @return Returns the status set in data->ret, may exit directly on failure.
 */
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

/**
 * Handles the execution of a non-builtin command by waiting for its completion.
 * 
 * @param[in] data Global data structure.
 * @param[in] args Command arguments.
 * @param[in] pid Process ID of the forked process executing the command.
 * @return Exit status of the command or error signals.
 */
int	exec_nonbuiltin_and_wait(t_global *data, char **args, int pid)
{
	int	status;
	int	ret;

	ret = 1;
	status = 0;
	if (pid == -1)
		perror("minishell: fork");
	else if (pid == 0)
	{
		status = exec_nonbuiltin(args, data);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	if (WIFSIGNALED(status))
		ret = handle_signal_interrupt(status, TRUE);
	else if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	if (ret == 1 && status != 0)
		ret = status;
	return (ret);
}

/**
 * Executes a non-builtin command, managing redirection and process forking.
 * 
 * @param[in, out] data Global data structure with redirection information.
 * @param[in] args Command arguments.
 * @return Status code of command execution or fork process.
 */
int	exec_nonbuiltin_onfork(t_global *data, char **args)
{
	int		pid;
	int		ret;
	int		i;

	i = 0;
	ret = 1;
	while (data->hashtable[0]->redir[i])
	{
		if (ft_strncmp(data->hashtable[0]->redir[i], "<<", 2) == 0)
			write_in_heredoc(data, i, 0, &data->hashtable[0]->redir[i][2]);
		i++;
	}
	if (handle_redirects(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (ret);
	}
	if (args && args[0] && !is_builtin(args[0]))
	{
		pid = fork();
		define_exec_signals(pid);
		ret = exec_nonbuiltin_and_wait(data, args, pid);
		restore_original_fds(data->original_fds);
	}
	return (ret);
}

/**
 * Manages the execution of a single process, either builtin or non-builtin.
 * 
 * @param[in, out] data Global data structure.
 * @return Status code of the executed process.
 */
int	exec_one_process(t_global *data)
{
	int		ret;
	char	**args;

	ret = 1;
	args = NULL;
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (args && args[0] && is_builtin(args[0]))
		ret = exec_builtin(args, hashsize(data->hashtable[0]), data);
	else
		ret = exec_nonbuiltin_onfork(data, args);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	restore_original_fds(data->original_fds);
	return (ret);
}
