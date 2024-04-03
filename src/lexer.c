#include "../includes/minishell.h"

bool	input_starts_with_command(t_tkn	*node)
{
	if ((!node->prev && !ft_strcmp(node->type, COMMAND))
		|| (node->prev && ft_strcmp(node->type, COMMAND)
			&& (!ft_strcmp(node->prev->content, PIPE)
				|| !ft_strcmp(node->prev->content, LOGIC_AND)
				|| !ft_strcmp(node->prev->content, LOGIC_AND))))
		return (false);
	return (true);
}

int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!input_starts_with_command((*hashtable)[i]))
			{
				printf("%s: command not found\n", (*hashtable)[i]->content);
				(*hashtable)[i] = temp;
				return (0);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return (1);
}
