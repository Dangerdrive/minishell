#include "../includes/minishell.h"

bool	input_starts_with_command(t_tkn	*node, int i)
{
	if ((i == 0 && !node->prev && ft_strcmp(node->type, COMMAND) != 0)
		|| (i > 0 && node->prev && !ft_strcmp(node->prev->content, PIPE)
			&& ft_strcmp(node->type, COMMAND)))
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
			if (!input_starts_with_command((*hashtable)[i], i))
			{
				printf("%s: command not found\n", (*hashtable)[i]->content);
				return (0);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return (1);
}
