#include "../includes/minishell.h"

char	*get_tkn_type(t_tkn *node)
{
	if (!node->type)
	{
		if (node->content[0] == SIMPLE_QUOTE)
			return (STRING_STRONG);
		else if (node->content[0] == DOUBLE_QUOTE)
			return (STRING_STD);
		else if (is_special_token(node->content))
			return (SPECIAL_CHAR);
		else if ((node->content[0] == '$' && identifier_is_valid(node->content + 1))
			|| !strcmp(node->content, "$?") || is_special_variable(node->content))
			return (VARIABLE);
		else if (!node->prev || (node->prev && !ft_strcmp(node->prev->content, PIPE)))
			return (COMMAND);
		else
			return (ARGUMENT);
	}
	return (node->type);
}

bool	check_there_is_var(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '$' && (ft_isalpha(content[i + 1]) || content[i + 1] == '_'))
			return (true);
		i++;
	}
	return (false);
}

bool	is_empty_str(char *content, char quote)
{
	int	i;

	i = 1;
	while (content[i] && content[i] != quote)
	{
		if (content[i] != 32)
			return (false);
		i++;
	}
	return (true);
}

void	update_content(char *content)
{
	char *new_content;
	int	len;
	int	i;

	len = ft_strlen(content) - 1;
	if (ft_strcmp(content, PIPE)
		&& (content[0] == 34 || content[0] == 39))
	{
		new_content = ft_calloc(len, sizeof(char));
		i = 1;
		while (i < len)
		{
			new_content[i - 1] = content[i];
			i++;
		}
		ft_strlcpy(content, new_content, len);
		free(new_content);
	}
}

// void	check_heredoc(t_tkn **node)
// {
// 	if (strncmp((*node)->content, DOUBLE_LESS_THAN))
// }

int	parse(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data)
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
			(*hashtable)[i]->type = get_tkn_type((*hashtable)[i]);
			//check_heredoc(&(*hashtable)[i]);
			update_content((*hashtable)[i]->content);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	syntax = 0;
	if (expand(hashtable, data) == 1)
		syntax = lexer(hashtable);
	return (syntax);
}
