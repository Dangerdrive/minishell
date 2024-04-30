#include "../includes/minishell.h"

// t_bool	input_starts_with_command(t_tkn	*node, int i)
// {
// 	if ((i == 0 && !node->prev
// 			&& !is_redir(node->content) && ft_strcmp(node->type, COMMAND))
// 		|| (i > 0 && node->prev && is_pipe(node->prev->content)
// 			&& ft_strcmp(node->type, COMMAND) && !is_redir_in(node->content)))
// 		return (FALSE);
// 	return (TRUE);
// }

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
