#include "../../includes/minishell.h"

static t_bool	argument_is_valid(char *arg)
{
	int	i;
	int	len_limit;

	len_limit = 19;
	i = 0;
	if (arg == NULL)
		return (0);
	if (arg[0] == '-' || arg[0] == +)
	{
		i++;
		len_limit++;
	}
	if (arg[i] == '9' && arg[i + 1] > '2')
		len_limit--;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);			
		i++;
	}
	if (i > len_limit)
		return (0);
	return (1);
}

void	ft_exit(char **args, int args_len, t_global *data)
{
	if (args_len == 1)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		data->ret = 2;
		data->exit = TRUE;
	}
	if (!argument_is_valid(args[1]))
	{
		data->ret = 2;
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		data->exit = TRUE;
	}
	if (argument_is_valid(args[1]) && args_len > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		data->ret = 1;
	}
	if (argument_is_valid(args[1]) && args_len == 2)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		data->ret = ft_atoi(args[1]);
		data->exit = TRUE;
	}
}

//needs testing