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

// int	copy_env(t_global **data)
// {
// 	int	i;
// 	int	env_size;

// 	env_size = ft_strarray_len(__environ);
// 	(*data)->env = malloc((env_size + 1) * sizeof(char *));
// 	if (!(*data)->env)
// 		return (EXIT_FAILURE);
// 	i = 0;
// 	while (i < env_size)
// 	{
// 		(*data)->env[i] = ft_strdup(__environ[i]);
// 		if (!(*data)->env[i])
// 		{
// 			while (i > 0)
// 			{
// 				i--;
// 				free((*data)->env[i]);
// 			}
// 			free((*data)->env);
// 			return (EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// 	(*data)->env[env_size] = NULL;
// 	return (EXIT_SUCCESS);
// }

// t_env *new_env_node(const char *key, const char *value)
// {
// 	t_env *node = malloc(sizeof(t_env));
// 	if (!node) return NULL;

// 	node->key = strdup(key);
// 	if (!node->key)
// 	{
// 		free(node);
// 		return NULL;
// 	}
// 	node->value = strdup(value);
// 	if (!node->value)
// 	{
// 		free(node->key);
// 		free(node);
// 		return NULL;
// 	}
// 	node->next = NULL;
// 	return node;
// }

void free_env(t_env **env)
{
	t_env *current;
	t_env *next;

	if (!env || !(*env))
		return;
	current = *env;
	while (current != NULL)
	{
		next = current->next;
		if (current->key)
			ft_memdel((void **)&(current->key));
		if (current->value)
			ft_memdel((void **)&(current->value));
		ft_memdel((void **)&current);
		current = next;
	}
	*env = NULL;
}

void append_env_node(t_env **env, t_env *new_node)
{
	t_env *temp;
	if (!env || !new_node)
		return;
	if (!(*env))
	{
		*env = new_node;
		return;
	}	
	temp = *env;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
	
}

int copy_env(t_global **data)
{
	int		i;
	char	*delim_pos;
	t_env 	*new_node;

	i = 0;
	while (__environ[i])
	{
		delim_pos = ft_strchr(__environ[i], '=');
		new_node = malloc(sizeof(t_env));
		if (!new_node)
		{
			free_env(&((*data)->env));
			return (1);
		}
		new_node->key = ft_strndup(__environ[i], delim_pos - __environ[i]);
		new_node->value = strdup(delim_pos + 1);
		// if (!new_node->key || !new_node->value)
		// {
		// 	free_env(&((*data)->env));
		// 	return (1);
		// }
		append_env_node(&((*data)->env), new_node);
		i++;
	}
	return (0);
}


int				copy_env(t_global *data)
{
	t_env	*env;
	t_env	*new;
	int		i;

	if (!(env = malloc(sizeof(t_env))))
		return (1);
	env->value = ft_strdup(__environ[0]);
	env->next = NULL;
	data->env = env;
	i = 1;
	while (__environ && __environ[0] && __environ[i])
	{
		if (!(new = malloc(sizeof(t_env))))
			return (1);
		new->value = ft_strdup(__environ[i]);
		new->next = NULL;
		env->next = new;
		env = new; 
		i++;
	}
	return (0);
}

void sort_env(t_global **data)
{
	int ordered;
	t_env	**current;
	t_env	*temp;
	t_env	*next_node;

	ordered = 0;
	if (!data || !(*data) || !(*data)->env)
		return;
	while (!ordered)
	{
		ordered = 1;
		current = &((*data)->env);
		while ((*current) != NULL && (*current)->next != NULL)
		{
			if (ft_strcmp((*current)->key, (*current)->next->key) > 0)
			{
				temp = *current;
				next_node = (*current)->next;
				temp->next = next_node->next;
				next_node->next = temp;
				*current = next_node;
				ordered = 0;
			}
			current = &((*current)->next);
		}
	}
}

int		env(t_env *env)
{
	while (env && env->next != NULL)
	{
		ft_printf("%s\n", env->value);
		env = env->next;
	}
	if (env)
		ft_printf("%s\n", env->value);
	return (0);
}


// void sort_env(t_global **data)
// {
//     int ordered;
//     int i;
//     int env_len;
//     char *tmp;

//     env_len = ft_strarray_len((*data)->env);
//     ordered = 0;
// 	(*data)->sorted_env = ft_strarray_dup((*data)->env);
//     while (!ordered)
// 	{
//         ordered = 1;
//         i = 0;
//         while (i < env_len - 1) {
//             if (ft_strcmp((*data)->sorted_env[i], (*data)->sorted_env[i + 1]) > 0) {
//                 tmp = (*data)->sorted_env[i];
//                 (*data)->sorted_env[i] = (*data)->sorted_env[i + 1];
//                 (*data)->sorted_env[i + 1] = tmp;
//                 ordered = 0;
//             }
//             i++;
//         }
//         env_len--;
//     }

// }

// int	main(void)
// {
// 	static t_global	*data;

// 	data = init_data();
// 	copy_env(&data);
// 	sort_env(&data);
// 	print_env(data->sorted_env);
// 	free(data);
// 	free(data->cur_path);
// }