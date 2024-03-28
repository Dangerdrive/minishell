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

bool	check_var_btw_simple_quote(char *content)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (content[i])
	{
		if (content[i] == 39)
			count++;
		if (count == 1 && content[i] == '$')
			count++;
		if (count == 2 && content[i] == 39)
			count++;
		i++;
	}
	if (count == 3)
		return (true);
	return (false);
}

// void	add_node_later(t_tkn **node)
// {
// 	t_tkn	*new_node;
// 	int		i;
// 	int		q_len;

// 	new_node = ft_calloc(1, sizeof(t_tkn));
// 	if (!new_node)
// 		return ;
// 	while ((*node)->content[i] != 39)
// 		i++;
// 	new_node->content = ft_strndup((*node)->content + i);
// 	new_node->type = STRING_STD;
// 	new_node->next = (*node)->next;
// 	new_node->prev = *node;
// 	if ((*node)->next)
// 		(*node)->next->prev = new_node;
// 	free((*node)->content);
// 	(*node)->content = *exp_value;
// 	(*node)->next = new_node;
// }

void	update_content(t_tkn **node)
{
	char *new_content;
	int	len;
	int	i;

	if ((*node)->content[0] == 34 || (*node)->content[0] == 39)
	{
		len = ft_strlen((*node)->content) - 1;
		new_content = ft_calloc(len, sizeof(char));
		i = 1;
		while (i < len)
		{
			new_content[i - 1] = (*node)->content[i];
			i++;
		}
		free((*node)->content);
		(*node)->content = new_content;
	}
}

int	parse(t_tkn *(*hashtable)[TABLE_SIZE], char **env)
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
			update_content(hashtable[i]);
			// if (ft_strcmp((*hashtable)[i]->type, STRING_STD)
			// 	&& check_var_btw_simple_quote((*hashtable)[i]->content))
			// 	add_nodes_later(hashtable[i]);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	expand(hashtable, env);
	syntax = check_syntax(hashtable);
	if (syntax)
		syntax = lexer(hashtable);
	return (syntax);
}
