#include "../includes/minishell.h"

int	is_special_token(char *token)
{
	if (token
		&& ((ft_strncmp(token, PIPE, 1) == 0)
			|| (ft_strncmp(token, AMPERSAND, 1) == 0)
			|| (ft_strncmp(token, GREATER_THAN, 1) == 0)
			|| (ft_strncmp(token, LESS_THAN, 1) == 0)
			|| (ft_strncmp(token, LOGIC_OR, 2) == 0)
			|| (ft_strncmp(token, LOGIC_AND, 2) == 0)
			|| (ft_strncmp(token, DOUBLE_GREATER_THAN, 2) == 0)
			|| (ft_strncmp(token, DOUBLE_LESS_THAN, 2) == 0)))
		return (1);
	else
		return (0);
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

void	parse(t_tkn *(*hashtable)[TABLE_SIZE])
{
	int	i;
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
}