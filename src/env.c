#include "../includes/minishell.h"

// void	print_our_env(t_global **data)
// {
// 	int	i;

// 	i = 0;
// 	while ((*data)->env[i])
// 	{
// 		printf("%s\n", (*data)->env[i]);
// 		free((*data)->env[i]);
// 		i++;
// 	}
// 	free((*data)->env[i]);
// 	free((*data)->env);
// }

void	print_our_env(t_global **data)
{
	int	i;

	i = 0;
	while ((*data)->env[i])
	{
		//printf("%s\n", (*data)->env[i]);
		i++;
	}
	free_env(data);
}

void	free_env(t_global **data)
{
	int	i;

	if (!(*data)->env)
		return ;
	i = 0;
	while ((*data)->env[i])
	{
		ft_memdel((*data)->env[i]);
		i++;
	}
	ft_memdel((*data)->env[i]);
	ft_memdel((*data)->env);
}

int	copy_env(t_global **data)
{
	int	i;
	int	env_size;

	env_size = 0;
	while (__environ[env_size])
		env_size++;
	(*data)->env = malloc((env_size + 1) * sizeof(char *));
	if (!(*data)->env)
		return (EXIT_FAILURE);
	i = 0;
	while (i < env_size)
	{
		(*data)->env[i] = ft_strdup(__environ[i]);
		if (!(*data)->env[i])
		{
			while (i > 0)
			{
				i--;
				free((*data)->env[i]);
			}
			free((*data)->env);
			return (EXIT_FAILURE);
		}
		i++;
	}
	(*data)->env[env_size] = NULL;
	//print_our_env(data); // for testing
	return (EXIT_SUCCESS);
}

// int	main(void)
// {
// 	static t_global	*data;

// 	data = init_data();
// 	copy_env(&data);
// 	free(data);
// 	free(data->cur_path);
// }