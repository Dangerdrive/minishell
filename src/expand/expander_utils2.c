#include "../includes/minishell.h"

/**
 * Searches for the key that matches the
 * token's variable and then returns its value.
 */
static char	*fetch_in_array(char **str, int i, int len, char *arr)
{
	char	*value;
	int		j;

	j = 0;
	if (ft_strncmp((*str) + i, arr, len) == 0)
	{
		while (arr[j] != '=')
			j++;
		if (j == len)
		{
			j++;
			value = ft_strdup(arr + j);
			return (value);
		}
	}
	return (NULL);
}

char	*search_value(t_global **data, char **str, int i, int len)
{
	char	*value;
	int		j;

	value = NULL;
	j = 0;
	while (!value && (*data)->env[j])
	{
		value = fetch_in_array(str, i, len, (*data)->env[j]);
		j++;
	}
	j = 0;
	if ((*data)->exported)
	{
		while (!value && (*data)->exported[j])
		{
			value = fetch_in_array(str, i, len, (*data)->exported[j]);
			j++;
		}
	}
	return (value);
}

void	handle_expand_fail(t_tkn **node)
{
	t_tkn	*temp;

	temp = *node;
	while (temp->prev)
		temp = temp->prev;
	if (ft_strcmp(temp->content, "echo") == 0)
	{
		free((*node)->content);
		temp = (*node)->prev;
		temp->next = (*node)->next;
		if ((*node)->next)
			(*node)->next->prev = temp;
		if (temp->space_after != (*node)->space_after)
			temp->space_after = (*node)->space_after;
		free(*node);
		*node = temp;
	}
	if ((*node)->prev && (ft_strcmp((*node)->prev->content, ">") == 0
			|| ft_strcmp((*node)->prev->content, "<") == 0))
		ft_printf("\n%sminishell: %s: ambiguous redirect%s\n",
			RED, (*node)->content, END);
}
