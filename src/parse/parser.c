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
		else if (is_export_var(node->content))
			return (EXPT_VARIABLE);
		else if (!node->prev)
			return (COMMAND);
		else
			return (ARGUMENT);
	}
	return (node->type);
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
	char	*new_content;
	t_tkn	*temp;

	if ((*node)->content)
	{
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
