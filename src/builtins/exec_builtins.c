#include "../../includes/minishell.h"

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

int	exec_builtin(char **args, int args_len, t_global *data)
{
	int		result;

	result = 0;
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
		result = ft_exit(args, args_len, data);
	return (result);
}
