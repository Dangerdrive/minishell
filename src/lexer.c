#include "../includes/minishell.h"

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
			if ((i == 0 && !(*hashtable)[i]->prev && ft_strcmp((*hashtable)[i]->type, COMMAND))
				|| (i > 0 && (*hashtable)[i]->prev && !ft_strcmp((*hashtable)[i]->prev->content, PIPE)
					&& ft_strcmp((*hashtable)[i]->type, COMMAND)))
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