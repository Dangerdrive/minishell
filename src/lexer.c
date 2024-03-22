#include "../includes/minishell.h"

int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		if (strcmp((*hashtable)[i]->type, COMMAND))
		{
			printf("%s: command not found\n", (*hashtable)[0]->content);
			return (0);
		}
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			// PRECISA VALIDAR SE ANTES DE UMA FLAG TEM UM COMANDO;
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return (1);
}