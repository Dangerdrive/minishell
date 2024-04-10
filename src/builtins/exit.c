#include "../../includes/minishell.h"

static t_bool	argument_is_valid(char *arg)
{
	int	i;
	int	len_limit;

	len_limit = 19;
	i = 0;
	if (arg == NULL)
		return (0);
	if (arg[0] == '-' || arg[0] == '+')
	{
		i++;
		len_limit++;
	}
	if (arg[i] == '9' && arg[i + 1] > '2')
		len_limit--;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (FALSE);
		i++;
	}
	if (i > len_limit)
		return (0);
	return (TRUE);
}

int	ft_exit(char **args, int args_len, t_global *data)
{
	if (args_len == 1)
	{
		ft_printf("exit\n");
		data->ret = 0;
		data->exit = TRUE;
	}
	else if (args_len > 1 && !argument_is_valid(args[1]))
	{
		data->ret = 2;
		ft_printf_fd(2, "exit: %s: numeric argument required\n", args[1]);
		data->exit = TRUE;
	}
	else if (args_len > 2 && argument_is_valid(args[1]))
	{
		ft_printf_fd(STDERR_FILENO, "minishell: exit: too many arguments\n");
		data->ret = 1;
	}
	else if (args_len == 2 && argument_is_valid(args[1]))
	{
		ft_printf("exit\n");
		data->ret = ft_atoi(args[1]);
		data->exit = TRUE;
	}
	return (data->ret);
}