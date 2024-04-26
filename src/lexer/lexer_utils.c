#include "../includes/minishell.h"

t_bool	input_starts_with_command(t_tkn	*node, int i)
{
	if ((i == 0 && !node->prev && !is_redir(node->content) && ft_strcmp(node->type, COMMAND))
		|| (i > 0 && node->prev && is_pipe(node->prev->content)
			&& ft_strcmp(node->type, COMMAND)))
		return (FALSE);
	return (TRUE);
}

void	check_redirects(t_tkn **node)
{
	t_tkn	*temp_node;
	t_tkn	*temp;

	temp = *node;
	temp_node = NULL;
	while (*node)
	{
		parse_redir(node, &temp_node);
		*node = (*node)->next;
	}
	if (temp_node && !temp_node->prev)
		*node = temp_node;
	else
	{
		*node = temp;
		while ((*node)->next != NULL)
			*node = (*node)->next;
		*node = temp_node;
		*node = temp;
	}
}

void	remove_pipe(t_tkn **node, int i)
{
	t_tkn	*temp;

	if (i > 0 && is_pipe((*node)->content))
	{
		temp = (*node)->next;
		free((*node)->content);
		free(*node);
		*node = temp;
		if (*node)
			(*node)->prev = NULL;
	}
}
