#include "../includes/minishell.h"

char	*ft_getenv(char *name, t_global **data)
{
	int		i;
	char	*value;

	i = 0;
	while ((*data)->env[i])
	{
		if (ft_strncmp((*data)->env[i], name, ft_strlen(name)) == 0)
		{
			value = ft_strchr((*data)->env[i], '=') + 1;
			return (value);
		}
		i++;
	}
	i = 0;
	while ((*data)->exported && (*data)->exported[i])
	{
		if (ft_strncmp((*data)->env[i], name, ft_strlen(name)) == 0)
		{
			value = ft_strchr((*data)->exported[i], '=') + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}

static int	env_print(t_global **data)
{
	int		i;
	t_bool	exp_ended;

	exp_ended = FALSE;
	if (!(*data)->env)
		return (1);
	i = 0;
	while ((*data)->env[i])
	{
		printf("%s\n", (*data)->env[i]);
		if (!exp_ended && (*data)->exported && (*data)->exported[i])
		{
			if (ft_strchr_i((*data)->exported[i], '=') != -1)
				printf("%s\n", (*data)->exported[i]);
			if (!(*data)->exported[i + 1])
				exp_ended = TRUE;
		}
		i++;
	}
	while (!exp_ended && (*data)->exported && (*data)->exported[i++ - 1])
		if (ft_strchr_i((*data)->exported[i], '=') != -1)
			printf("%s\n", (*data)->exported[i]);
	return (0);
}

int	ft_env(char **args, int args_len, t_global **data)
{
	if (args_len == 1)
		return (env_print(data));
	else if (args_len > 1)
	{
		if (access(args[1], F_OK) == 0)
			ft_dprintf(2, "env: %s: Permission denied\n", args[1]);
		else
			ft_dprintf(2, "env: %s: No such file or directory\n", args[1]);
		return (1);
	}
	return (0);
}

