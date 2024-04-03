#include "../includes/minishell.h"

// char	*fetch_on_env(char *env_value)
// {
// 	char	*result;

// 	while (*env_value != '=')
// 	{
// 		env_value++;
// 	}
// 	env_value++;
// 	result = ft_strdup(env_value);
// 	return (result);
// }

bool	is_special_char(char c)
{
	if ((c >= 32 && c <= 47 && c != 35) || (c >= 58 && c <= 64 && c != 63)
		|| (c >= 93 && c <= 96 && c != '_') || (c >= 123 && c <= 126))
	{
		return (true);
	}
	return (false);
}

void	add_node_before(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return ;
	new_node->content = ft_strndup((*node)->content, i - 1);
	new_node->type = STRING_STD;
	new_node->next = (*node);
	new_node->prev = (*node)->prev;
	(*node)->prev->next = new_node;
}

t_tkn	*add_node_after(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup((*node)->content + i);
	new_node->type = STRING_STD;
	new_node->next = (*node)->next;
	new_node->prev = *node;
	if ((*node)->next)
		(*node)->next->prev = new_node;
	return (new_node);
}

void	update_list(t_tkn **node, int i, int len, char **exp_value)
{
	t_tkn 	*new_node;

	new_node = NULL;
	if (i > 1)
	{
		add_node_before(node, i);
	}
	if ((*node)->content[len])
	{
		new_node = add_node_after(node, len);
	}
	free((*node)->content);
	(*node)->content = *exp_value;
	if (new_node)
	{
		(*node)->next = new_node;
	}
}

bool	is_var_name(t_tkn **node, int i, int len)
{
	if (((*node)->type[0] == 'v' && (*node)->content[i + len]
		&& !is_special_char((*node)->content[i + len]))
		|| ((*node)->type[0] == 's' && (*node)->content[i + len]
			&& !is_special_char((*node)->content[i + len])))
		return (true);
	return (false);
}

char	*fetch_in_array(t_tkn **node, int i, int len, char *arr)
{
	char	*value;

	if (ft_strncmp((*node)->content + i, arr, len) == 0)
	{
		while (*arr != '=')
			arr++;
		arr++;
		value = ft_strdup(arr);
		return (value);
	}
	return (NULL);
}

void	get_var_value(t_tkn **node, int i, char **env, char **exported)
{
	int		j;
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (is_var_name(node, i, len))
		len++;
	if (!ft_strcmp((*node)->content, "$?") || !ft_strcmp((*node)->content, "$#"))
		value = ft_strdup("0"); /* precisa mallocar senão dá invalid free() */
	j = 0;
	while (!value && env[j++])
		value = fetch_in_array(node, i, len, env[j]);
	j = 0;
	while (!value && exported[j++])
		value = fetch_in_array(node, i, len, exported[j]);
	if (value)
	{
		update_list(node, i, i + len, &value);
	}
}

void	check_if_expandable(t_tkn **node, char **env, char **exported)
{
	int		i;

	if (!ft_strcmp((*node)->type, VARIABLE) || !ft_strcmp((*node)->type, STRING_STD))
	{
		i = 0;
		while ((*node)->content[i] != '\0')
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				get_var_value(node, i, env, exported);
				break ;
			}
			i++;
		}
	}
	return ;
}

void	expand(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data)
{
	int		i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			check_if_expandable(&(*hashtable)[i], (*data)->env, (*data)->exported);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return ;
}


// TO TEST:
// echo "hello $USER"
// echo "hello $USER !!"
// echo 'hello $USER'
// echo '"hello $USER"'
// echo hello "'"$USER"'"
// echo "hello '$USER!' How are you?"
