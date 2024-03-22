#include "../includes/minishell.h"

int	check_syntax(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int 	i;
	t_tkn *temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if ((is_pipe_and_or((*hashtable)[i]->content) && !(*hashtable)[i]->next)
			|| ((*hashtable)[i] && (*hashtable)[i]->next && is_special_token((*hashtable)[i]->content) && is_special_token((*hashtable)[i]->next->content)))
			{
				printf("Syntax error.\n");
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

char	*get_tkn_type(char *token)
{
	int	i;

	i = 0;
	if (token[i] == SIMPLE_QUOTE)
		return (STRING_STRONG);
	else if (token[i] == DOUBLE_QUOTE)
		return (STRING_STD);
	else if (is_special_token(token))
		return (SPECIAL_CHAR);
	else if (token[i] == '-' && token[i + 1])
		return (FLAG);
	else
		return (COMMAND);

}

int	parse(t_tkn *(*hashtable)[TABLE_SIZE])
{
	int		i;
	int		syntax;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			(*hashtable)[i]->type = get_tkn_type((*hashtable)[i]->content);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	syntax = check_syntax(hashtable);
	if (syntax)
		syntax = lexer(hashtable);
	return (syntax);
}