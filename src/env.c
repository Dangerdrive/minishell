#include "../includes/minishell.h"

int	ft_env(t_global **data)
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
