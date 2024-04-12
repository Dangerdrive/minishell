#include "../includes/minishell.h"

bool	is_heredoc(char *content)
{
	if (strcmp(content, DOUBLE_LESS_THAN) == 0)
		return (true);
	return (false);
}

bool	is_double_special_token(t_tkn *node)
{
	if ((!node->next || is_special_token(node->next->content)) && is_special_token(node->content))
		return (true);
	return (false);
}

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
			if ((is_double_special_token((*hashtable)[i]) && !(*hashtable)[i]->delimiter)
				|| is_and_or((*hashtable)[i]->content))
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

bool	is_export_var(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '=' && !content[i + 1])
			return (true);
		i++;
	}
	return (false);
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
		else if ((node->content[0] == '$' && identifier_is_valid(node->content + 1))
			|| !strcmp(node->content, "$?") || is_special_variable(node->content))
			return (VARIABLE);
		else if (is_export_var(node->content))
			return (EXPT_VARIABLE);
		else if (!node->prev)
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

void	remove_quotes(t_tkn **node, char *content)
{
	char *new_content;
	int	len;
	int	i;

	(void)node;
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

void	check_pipe(t_tkn **node, int i)
{
	t_tkn	*temp;

	if (i > 0 && is_pipe((*node)->content))
	{
		temp = (*node)->next;
		free((*node)->content);
		free(*node);
		*node = temp;
		if (*node)
			(*node)->prev = NULL;
	}
}

void	check_export(t_tkn **node)
{
	char 	*new_content;
	t_tkn	*temp;

	if ((*node)->prev && ft_strcmp((*node)->prev->type, EXPT_VARIABLE) == 0
		&& ft_strcmp((*node)->type, SPECIAL_CHAR)) // CHECAR NECESSIDADE DESSA VERIFICAÇÃO
	{
		new_content = ft_strjoin((*node)->prev->content, (*node)->content);
		free((*node)->content);
		(*node)->content = ft_strdup(new_content);
		temp = (*node)->prev->prev;
		if (temp)
			temp->next = *node;
		free(new_content);
		free((*node)->prev->content);
		free((*node)->prev);
		(*node)->prev = temp;
	}
}

void	update_redir_files_list(char **redir_list, char *new_file)
{
	int	i;

	i = 0;
	if (redir_list[0])
	{
		while (redir_list[i])
		i++;
	}
	redir_list[i] = ft_strdup(new_file);
}

t_bool	is_redirect(char *c)
{
	if (ft_strcmp(c, GREATER_THAN))
		return (true);
	return (false);
}

void	check_redirects(t_tkn **node)
{
	t_tkn	*temp_node;
	t_tkn	*temp_tkn;

	if (is_redirect((*node)->prev->content))
	{
		temp_node = (*node)->prev;
		while (*node)
		{
			if (is_redirect((*node)->prev->content))
			{
				update_redir_files_list((*node)->prev->args, (*node)->content);
				free((*node)->content);
				temp_tkn = (*node)->prev;
				temp_tkn->next = (*node)->next;
				(*node)->next->prev = temp_tkn;
				free(*node);
				(*node) = temp_tkn;
			}
			*node = (*node)->next;
		}
		*node = temp_node;
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
		check_pipe(&(*hashtable)[i], i);
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			(*hashtable)[i]->type = get_tkn_type((*hashtable)[i]);
			remove_quotes(hashtable[i], (*hashtable)[i]->content);
			check_export(hashtable[i]);
			check_redirects(hashtable[i]);
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
