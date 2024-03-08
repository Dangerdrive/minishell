#include "../includes/minishell.h"

// int	copy_env(t_global	*data)
// {
// 	int	i;

// 	i = 0;
// 	data.env = malloc(sizeof(char **));
// 	if (!data.env)
// 		return (EXIT_FAILURE);
// 	while (__environ)
// 	{
// 		data.env[i] = ft_strdup(__environ[i]);
// 		i+

// }

int	copy_env(t_global *data)
{
	int	i;
	int	env_size;

	env_size = 0;
	while (__environ[env_size])
		env_size++;
	data->env = malloc((env_size + 1) * sizeof(char *));
	if (!data->env)
		return (EXIT_FAILURE);
	i = 0;
	while (i < env_size)
	{
		data->env[i] = ft_strdup(__environ[i]);
		if (!data->env[i])
		{
			while (i > 0)
			{
				i--;
				free(data->env[i]);
			}
			free(data->env);
			return (EXIT_FAILURE);
		}
		i++;
	}
	data->env[env_size] = NULL;
	return (EXIT_SUCCESS);
}
