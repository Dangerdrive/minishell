#include "../includes/minishell.h"

void	*subscribe_to_gc(t_list **gc, void **item)
{
	t_list	*new_node;

	new_node = ft_calloc(1, sizeof(t_list));
	new_node->content = *item;
	if (!(*gc))
		*gc = new_node;
	else
	{
		while ((*gc)->next)
			gc = (*gc)->next;
		(*gc)->next = new_node;
	}
}
