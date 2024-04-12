#include "../includes/minishell.h"

/**
 * It updates the token node with the variable value (*exp_value).
 */
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

/**
 * Searches for the key that matches the token's variable and then returns its value.
 */
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

/**
 * Searches for the variable value in the arrays (*data)->env and (*data)->exported.
 * Then, it updates the token hashtable with the founded value.
 *
 * Returns (1) if no problem is found.
 * Otherwise, returns (0).
 */
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
		value = ft_itoa((*data)->prev_process_status);
	if (is_special_variable((*node)->content))
	{
		printf("%s\nThis functionality is beyond Minishell's scope, ****@#$@***.\n\n%s", RED, END);
		return (0);
	}
	j = 0;
	while (!value && (*data)->env[j])
	{
		value = fetch_in_array(node, i, len, (*data)->env[j]);
		j++;
	}
	j = 0;
	while (!value && (*data)->exported[j])
	{
		value = fetch_in_array(node, i, len, (*data)->exported[j]);
		j++;
	}
	if (!value)
	{
		printf("\n");
		return (0);
	}
	else
		update_list(node, i, i + len, &value);
	return (1);
}

/**
 * Check if the token contains a variable to be expanded.
 *
 * Returns (1) if no problem is found.
 * Otherwise, returns (0).
 */
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

/**
 * Handles variable expansion.
 *
 * Returns (1) if no problem is found during expansion.
 * Otherwise, returns (0).
 */
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
