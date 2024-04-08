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


int	ft_env(t_global **data)
{
	int	i;

	if (!(*data)->env)
		return (1);
	i = 0;
	while ((*data)->env[i])
	{
		printf("%s\n", (*data)->env[i]);
		if ( (*data)->exported && (*data)->exported[i])
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



// int				charenv_to_envlist(char **env)
// {
// 	t_env	*env;
// 	t_env	*new;
// 	int		i;

// 	env = malloc(sizeof(t_env));
// 	if (!env)
// 		return (1);
// 	// env->value = ft_strdup(env[0]);
// 	// env->next = NULL;
// 	data->env = env;
// 	i = 0;
// 	// while (env && env[0] && env[i])
// 	while (env && env[i])
// 	{
// 		new = malloc(sizeof(t_env));
// 		if (!new)
// 			return (1);
// 		new->value = ft_strdup(env[i]);
// 		new->next = NULL;
// 		env->next = new;
// 		env = new;
// 		i++;
// 	}
// 	ft_env(data->env);//
// 	return (0);
// }


// t_env   *new_env_node(char *key, char *value)
// {
// 	t_env   *node;

// 	node = (t_env *)malloc(sizeof(t_env));
// 	if (!node)
// 		return (NULL);
// 	node->key = strdup(key);
// 	if (!(node->key))
// 	{
// 		free(node);
// 		return (NULL);
// 	}
// 	node->value = strdup(value);
// 	if (!(node->value))
// 	{
// 		free(node->key);
// 		free(node);
// 		return (NULL);
// 	}
// 	node->next = NULL;
// 	return (node);
// }










// int	main(void)
// {
// 	static t_global	*data;

// 	data = init_data();
// 	copy_env(&data);
// 	free(data);
// 	free(data->cur_path);
// }