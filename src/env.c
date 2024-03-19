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

void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
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

	env_size = ft_strarray_len(__environ);
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
	return (EXIT_SUCCESS);
}

char	**ft_strarray_dup(char **array)
{
	int		i;
	char	**dup;

	i = 0;
	while (array[i])
		i++;
	dup = ft_calloc(1, (i + 1) * sizeof(char *));
	if (!dup)
		return (NULL);
	i = 0;
	while (array[i])
	{
		dup[i] = ft_strdup(array[i]);
		if (!dup[i])
		{
			while (i > 0)
			{
				i--;
				free(dup[i]);
			}
			free(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}	

// "export" shows environment variables in alphabetical order, but it has 1 line less than envp
void sort_env(t_global **data)
{
    int ordered;
    int i;
    int env_len;
    char *tmp;

    env_len = ft_strarray_len((*data)->env); // Corrected access to data's members
    ordered = 0;
	(*data)->sorted_env = ft_strarray_dup((*data)->env);
    // (*data)->sorted_env = (char **)ft_calloc(env_len + 1, sizeof(char *));
    // if (!(*data)->sorted_env)
    //     return;
    // i = 0;
    // while (i < env_len) {
    //     (*data)->sorted_env[i] = ft_strdup((*data)->env[i]); // Corrected access
    //     i++;
    // }
    // (*data)->sorted_env[env_len] = NULL;
    while (!ordered)
	{
        ordered = 1;
        i = 0;
        while (i < env_len - 1) {
            if (ft_strcmp((*data)->sorted_env[i], (*data)->sorted_env[i + 1]) > 0) {
                tmp = (*data)->sorted_env[i];
                (*data)->sorted_env[i] = (*data)->sorted_env[i + 1];
                (*data)->sorted_env[i + 1] = tmp;
                ordered = 0;
            }
            i++;
        }
        env_len--; // This line is placed correctly; it gradually reduces the array's size being sorted.
    }

}


//    data->sorted_env = (char **)malloc(sizeof(char *) * (env_len + 1));
//     for (i = 0; i < env_len; i++) {
//         data->sorted_env[i] = data->env[i];
//     }
//     data->sorted_env[env_len] = NULL; // Null-terminate the array

//     // Now sort data->sorted_env
//     ordered = 0;
//     while (!ordered) {
//         ordered = 1;
//         for (i = 0; i < env_len - 1; i++) {
//             if (strcmp(data->sorted_env[i], data->sorted_env[i + 1]) > 0) {
//                 tmp = data->sorted_env[i];
//                 data->sorted_env[i] = data->sorted_env[i + 1];
//                 data->sorted_env[i + 1] = tmp;
//                 ordered = 0;
//             }
//         }
//         env_len--;
//     }
// }

int	main(void)
{
	static t_global	*data;

	data = init_data();
	copy_env(&data);
	sort_env(&data);
	print_env(data->sorted_env);
	free(data);
	free(data->cur_path);
}