#include "../includes/minishell.h"

/**
 * It updates the token node with the variable value (*exp_value).
 */
void	update_node(t_tkn **node, int i, int len, char **exp_value)
{
	int	node_len;
	int	value_len;
	char	*new_content;

	node_len = strlen((*node)->content) - (len - i);
	value_len = ft_strlen(*exp_value);
	new_content = ft_calloc((node_len + value_len + 1) * sizeof(char));
	// TO BE CONTINUED...
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
		printf("%s\nThis functionality is beyond Minishell's scope, M0therF@ck&r.\n\n%s", RED, END);
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
		update_node(node, i, i + len, &value);
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

void	check_heredoc(t_tkn **node)
{
	t_tkn	*temp;

	temp = NULL;
	if (strncmp((*node)->content, DOUBLE_LESS_THAN, 2) == 0
		&& (*node)->next && !is_special_token((*node)->next->content))
	{
		temp = (*node)->next->next;
		(*node)->delimiter = ft_strdup((*node)->next->content);
		free((*node)->next->content);
		free((*node)->next);
		(*node)->next = temp;
		if (temp)
			temp->prev = *node;
	}
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
			check_heredoc(&(*hashtable)[i]);
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
