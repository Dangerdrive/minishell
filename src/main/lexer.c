#include "../includes/minishell.h"

bool	input_starts_with_command(t_tkn	*node, int i)
{
	if ((i == 0 && !node->prev && ft_strcmp(node->type, COMMAND))
		|| (i > 0 && node->prev && ft_strcmp(node->type, COMMAND)
			&& is_pipe(node->prev->content)))
		return (false);
	return (true);
}

t_bool	is_redir_out(char *c)
{
	if (!ft_strcmp(c, GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_redir_in(char *c)
{
	if (!ft_strcmp(c, LESS_THAN))
		return (true);
	return (false);
}

t_bool	is_append(char *c)
{
	if (!ft_strcmp(c, DOUBLE_GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_heredoc(char *content)
{
	if (strcmp(content, DOUBLE_LESS_THAN) == 0)
		return (true);
	return (false);
}

void	update_redir_files_list(t_tkn **node, char *new_arg, char *sig)
{
	int		i;
	char	*new_sig;

	i = 0;
	if (is_redir_in(sig) || is_redir_out(sig))
		new_sig = ft_strjoin(sig, " ");
	else
		new_sig = ft_strdup(sig);
	if (!(*node)->redir[i])
	{
		(*node)->redir[i] = ft_strjoin(new_sig, new_arg);
		return ;
	}
	while ((*node)->redir[i])
		i++;
	(*node)->redir[i] = ft_strjoin(new_sig, new_arg);
	//free(new_sig);
}

t_bool	is_redir(char *sig)
{
	if (is_redir_in(sig) || is_redir_out(sig)
		|| is_heredoc(sig) || is_append(sig))
		return (true);
	return (false);
}

void	check_redirects(t_tkn **node)
{
	t_tkn	*temp_node;
	t_tkn	*temp_tkn;

	if ((*node)->prev && is_redir((*node)->prev->content))
	{
		temp_node = (*node)->prev->prev;
		init_redir_args(&(*node)->redir);
		while (*node)
		{
			if ((ft_strcmp((*node)->type, SPECIAL_CHAR)) && is_redir((*node)->prev->content))
			{
				update_redir_files_list(&temp_node, (*node)->content, (*node)->prev->content);
				free((*node)->content);
				temp_tkn = (*node)->prev;
				temp_tkn->next = (*node)->next;
				if ((*node)->next)
					(*node)->next->prev = temp_tkn;
				free(*node);
				free((*node)->prev->content);
				free((*node)->prev);
				(*node) = temp_tkn;
			}
			*node = (*node)->next;
		}
		*node = temp_node;
	}
}

int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!input_starts_with_command((*hashtable)[i], i))
			{
				printf("%s: command not found\n", (*hashtable)[i]->content);
				(*hashtable)[i] = temp;
				return (0);
			}
			check_redirects(hashtable[i]);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return (1);
}
