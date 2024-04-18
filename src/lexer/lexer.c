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
			if ((is_double_special_token((*hashtable)[i]) && !(*hashtable)[i]->delimiter)
				|| (!(*hashtable)[i]->next && !ft_strcmp((*hashtable)[i]->type, SPECIAL_CHAR))
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

t_bool	input_starts_with_command(t_tkn	*node)
{
	if (!node->prev
		&& ft_strcmp(node->type, COMMAND) && !is_redir(node->content))
		return (false);
	return (true);
}

void	update_redir_files_list(char *(*redir)[TABLE_SIZE], char *new_arg, char *sig)
{
	int		i;
	char	*new_sig;

	i = 0;
	if (is_redir_in(sig) || is_redir_out(sig))
		new_sig = ft_strjoin(sig, " ");
	else
		new_sig = ft_strdup(sig);
	if ((*redir)[i])
	{
		while ((*redir)[i])
			i++;
	}
	(*redir)[i] = ft_strjoin(new_sig, new_arg);
	free(new_sig);
	return ;
}

void	update_node_after_redir(t_tkn **node)
{
	t_tkn	*temp;

	free((*node)->content);
	if ((*node)->prev->prev)
		temp = (*node)->prev->prev;
	else
		temp = (*node)->prev;
	temp->next = (*node)->next;
	if ((*node)->next)
		(*node)->next->prev = temp;
	if ((*node)->prev)
	{
		free((*node)->prev->content);
		(*node)->prev->content = NULL;
	}
	if ((*node)->prev->prev)
		free((*node)->prev);
	free(*node);
	(*node) = temp;
}

void	check_redirects(t_tkn **node)
{
	t_tkn	*temp_node;

	if ((*node)->prev && (*node)->prev->content && is_redir((*node)->prev->content))
	{
		if ((*node)->prev->prev)
			temp_node = (*node)->prev->prev;
		else
			temp_node = (*node)->prev;
		init_redir_args(&temp_node->redir);
		while (*node)
		{
			if ((ft_strcmp((*node)->type, SPECIAL_CHAR)) && is_redir((*node)->prev->content))
			{
				update_redir_files_list(&temp_node->redir, (*node)->content, (*node)->prev->content);
				update_node_after_redir(node);
			}
			*node = (*node)->next;
		}
		*node = temp_node;
	}
}

t_bool	check_valid_input(t_tkn **node, t_tkn *temp)
{
	if (!input_starts_with_command((*node)))
	{
		printf("%s: command not found\n", (*node)->content);
		(*node) = temp;
		return (FALSE);
	}
	return (TRUE);
}

void	remove_pipe(t_tkn **node, int i)
{
	t_tkn	*temp;

	if (i >= 0 && is_pipe((*node)->content))
	{
		temp = (*node)->next;
		free((*node)->content);
		free(*node);
		*node = temp;
		if (*node)
			(*node)->prev = NULL;
	}
}

int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	t_tkn	*temp;
	int 	i;

	i = 0;
	if (check_syntax(hashtable) != 1)
		return (0);
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!check_valid_input(hashtable[i], temp))
				return (0);
			check_redirects(hashtable[i]);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		remove_pipe(&(*hashtable)[i], i);
		i++;
	}
	return (1);
}
