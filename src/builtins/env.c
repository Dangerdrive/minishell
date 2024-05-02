#include "../includes/minishell.h"

/**
 * Retrieves the value of an environment variable from either the 'env' or
 * 'exported' arrays in the global data structure.
 * 
 * @param[in] name The name of the environment variable to find.
 * @param[in] data Pointer to the global data structure.
 * @return The value of the environment variable if found, NULL otherwise.
 */
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
		if (ft_strncmp((*data)->exported[i], name, ft_strlen(name)) == 0)
		{
			value = ft_strchr((*data)->exported[i], '=') + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}

/**
 * Prints all environment variables from the 'env' and 'exported' arrays
 * in the global data structure.
 * 
 * @param[in] data Pointer to the global data structure.
 * @return 0 on success, 1 if the environment array is empty.
 */
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

/**
 * Executes the 'env' command to print environment variables, handling
 * errors and permissions for additional arguments.
 * 
 * @param[in] args The arguments passed to the env command.
 * @param[in] args_len The number of arguments.
 * @param[in, out] data Pointer to the global data structure.
 * @return 0 on success, 1 on error or if arguments are invalid.
 */
int	ft_env(char **args, int args_len, t_global **data)
{
	if (args_len == 1)
		return (env_print(data));
	else if (args_len > 1)
	{
		if (access(args[1], F_OK) == 0)
		{
			ft_dprintf(2, "env: %s: Permission denied\n", args[1]);
			(*data)->ret = PERMISSION_DENIED;
		}
		else
		{
			ft_dprintf(2, "env: %s: No such file or directory\n", args[1]);
			(*data)->ret = 1;
		}
		return (1);
	}
	return (0);
}
