#include "../../includes/minishell.h"

/**
 * Determines if a given command string matches any known shell builtin commands.
 * 
 * @param[in] command The command string to check.
 * @return 1 (TRUE) if the command is a builtin, otherwise 0 (FALSE).
 */
t_bool	is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

/**
 * Executes a shell builtin command based on the parsed arguments.
 * Manages redirection and restores original file descriptors.
 * 
 * @param[in] args Array of arguments including the command.
 * @param[in] args_len Number of arguments passed.
 * @param[in, out] data Global data context including file descriptors.
 * @return 0 on success or 1 on failure to execute the command or manage 
 * redirections.
 */
int	exec_builtin(char **args, int args_len, t_global *data)
{
	int		result;

	result = 0;
	if (handle_redirects(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (1);
	}
	if (ft_strcmp(args[0], "echo") == 0)
		result = ft_echo(args, args_len);
	if (ft_strcmp(args[0], "cd") == 0)
		result = ft_cd(args, args_len, data);
	if (ft_strcmp(args[0], "pwd") == 0)
		result = ft_pwd();
	if (ft_strcmp(args[0], "env") == 0)
		result = ft_env(args, args_len, &data);
	if (ft_strcmp(args[0], "export") == 0)
		result = ft_export(args, args_len, data);
	if (ft_strcmp(args[0], "unset") == 0)
		result = ft_unset(args, args_len, data);
	if (ft_strcmp(args[0], "exit") == 0)
		result = ft_exit(args, ft_strarr_len(args), data);
	restore_original_fds(data->original_fds);
	return (result);
}
