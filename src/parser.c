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

int	is_file(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '.' && strcmp((token + i + 1), ".txt"))
			return (1);
		i++;
	}
	return (0);
}

char	*get_tkn_type(char *token, t_tkn *prev)
{
	if (token[0] == SIMPLE_QUOTE)
		return (STRING_STRONG);
	else if (token[0] == DOUBLE_QUOTE)
		return (STRING_STD);
	else if (is_special_token(token))
		return (SPECIAL_CHAR);
	else if (token[0] == '-' && token[1])
		return (FLAG);
	else if (token[0] == '$')
		return (VARIABLE);
	else if (prev && ft_strcmp(prev->type, COMMAND) == 0 && is_file(token))
		return (FILE_TXT);
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
			(*hashtable)[i]->type = get_tkn_type((*hashtable)[i]->content, (*hashtable)[i]->prev);
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