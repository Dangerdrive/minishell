#include "../includes/minishell.h"

bool	input_starts_with_command(t_tkn	*node, int i)
{
	if ((i == 0 && !node->prev && ft_strcmp(node->type, COMMAND))
		|| (i > 0 && node->prev && ft_strcmp(node->type, COMMAND)
			&& is_pipe(node->prev->content)))
		return (false);
	return (true);
}

// bool	ends_with_redirect(t_tkn *node, int i)
// {

// }

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
			if (!input_starts_with_command((*hashtable)[i], i))
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
