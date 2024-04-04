#include "../includes/minishell.h"

static int	env_print(t_global **data)
{
	int	i;

	if (!(*data)->env)
		return (1);
	i = 0;
	while ((*data)->env[i])
	{
		printf("%s\n", (*data)->env[i]);
		if ((*data)->exported && (*data)->exported[i])
			if (ft_strchr_i((*data)->exported[i], '=') != -1)
				printf("%s\n", (*data)->exported[i]);
		i++;
	}
	while ((*data)->exported && (*data)->exported[i])
	{
		if (ft_strchr_i((*data)->exported[i], '=') != -1)
			printf("%s\n", (*data)->exported[i]);
		i++;
	}
	return (0);
}

int	ft_env(char **args, int args_len, t_global **data)
{
	if (args_len == 1)
		return (env_print(data));
	else if (args_len > 1)
	{
		if (access(args[1], F_OK) == -1)
		{
			if (errno == EACCES)
			{
				ft_putstr_fd("env: ", STDERR_FILENO);
				ft_putstr_fd(args[1], STDERR_FILENO);
				ft_putendl_fd(": Permission denied", STDERR_FILENO);
			}
			else
			{
				ft_putstr_fd("env: ", STDERR_FILENO);
				ft_putstr_fd(args[1], STDERR_FILENO);
				ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			}
			return (1);
		}
	}
	return (0);
}

