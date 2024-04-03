#include "../includes/minishell.h"

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

bool	is_special_variable(char *var)
{
	if (!ft_strcmp(var, "$#")  || !ft_strcmp(var, "$!") || !ft_strcmp(var, "$@")
		|| !ft_strcmp(var, "$$") || !ft_strcmp(var, "$0") || !ft_strcmp(var, "$-")
		|| !ft_strcmp(var, "$*"))
	{
		return (true);
	}
	return (false);
}

int	get_var_value(t_tkn **node, int i, t_global **data)
{
	int		j;
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (is_var_name(node, i, len))
		len++;
	if (!ft_strcmp((*node)->content, "$?"))
		value = ft_itoa((*data)->prev_process);
	if (is_special_variable((*node)->content))
	{
		printf("%s\nThis functionality is beyond Minishell's scope, M0therF@ck&r.\n\n%s", RED, END);
		return (0);
	}
	j = 0;
	while (!value && (*data)->env[j++])
		value = fetch_in_array(node, i, len, (*data)->env[j]);
	j = 0;
	while (!value && (*data)->exported[j++])
		value = fetch_in_array(node, i, len, (*data)->exported[j]);
	if (value)
		update_list(node, i, i + len, &value);
	return (1);
}

int	check_if_expandable(t_tkn **node, t_global **data)
{
	int		i;
	int		result;

	result = 1;
	if (!ft_strcmp((*node)->type, VARIABLE) || !ft_strcmp((*node)->type, STRING_STD))
	{
		i = 0;
		while ((*node)->content[i] != '\0')
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				result = get_var_value(node, i, data);
				break ;
			}
			i++;
		}
	}
	return (result);
}

int	expand(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data)
{
	int		i;
	int		result;
	t_tkn	*temp;

	i = 0;
	result = 1;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			result = check_if_expandable(&(*hashtable)[i], data);
			if (result == 0)
			{
				(*hashtable)[i] = temp;
				return (result);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return (result);
}
