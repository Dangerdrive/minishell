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
			if (((*hashtable)[i] && (*hashtable)[i]->next
				&& is_special_token((*hashtable)[i]->content)
				&& is_special_token((*hashtable)[i]->next->content))
				|| (is_and_or((*hashtable)[i]->content)))
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
		else if ((node->content[0] == '$' && validate_identifier(node->content + 1))
			|| !strcmp(node->content, "$?") || is_special_variable(node->content))
			return (VARIABLE);
		else if (!node->prev || !strcmp(node->prev->content, PIPE))
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

void	split_token(t_tkn **node)
{
	t_tkn	*new_node;
	t_tkn	*new_node2;
	char	*temp;
	int		i;
	int		len;

	temp = (*node)->content;
	i = 0;
	while (temp[i])
	{
		if (i > 0 && temp[i] == 39)
		{
			i++;
			len = 0;
			while (temp[i + len] != 39)
				len++;
			new_node = ft_calloc(1, sizeof(t_tkn));
			new_node->content = ft_strndup((*node)->content + i, len);
			new_node->type = STRING_STRONG;
			new_node->next = (*node)->next;
			new_node->prev = (*node);
			if ((*node)->next)
				(*node)->next->prev = new_node;
			(*node)->next = new_node;
			free((*node)->content);
			(*node)->content = ft_strndup(temp, i - 1);
			new_node2 = ft_calloc(1, sizeof(t_tkn));
			new_node2->content = ft_strdup(temp + len);
			new_node2->next = new_node->next;
			new_node2->prev = new_node;
			new_node->next = new_node2;
			new_node2->next->prev = new_node2;
		}
	}
}

void	update_content(t_tkn **node, char *content)
{
	char *new_content;
	int	len;
	int	i;

	if (ft_strcmp(content, PIPE) && (content[0] == 34 || content[0] == 39))
	{
		len = ft_strlen(content) - 1;
		new_content = ft_calloc(len, sizeof(char));
		i = 1;
		while (i < len)
		{
			new_content[i - 1] = content[i];
			i++;
		}
		free(content);
		content = new_content;
		if (ft_strcmp((*node)->type, STRING_STD)
			&& check_there_is_var(content))
			split_token(node);
	}
}

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
			update_content(hashtable[i], (*hashtable)[i]->content);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	syntax = 0;
	if (expand(hashtable, data) == 1 && check_syntax(hashtable) == 1)
		syntax = lexer(hashtable);
	return (syntax);
}
