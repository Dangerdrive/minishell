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

void	check_redirects(t_tkn **node)
{
	t_tkn	*temp_node;
	t_tkn	*temp_tkn;

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
				update_redir_files_list(&temp_node, (*node)->content, (*node)->prev->content);
				free((*node)->content);
				if ((*node)->prev->prev)
					temp_tkn = (*node)->prev->prev;
				else
					temp_tkn = (*node)->prev;
				temp_tkn->next = (*node)->next;
				if ((*node)->next)
					(*node)->next->prev = temp_tkn;
				free(*node);
				free((*node)->prev->content);
				(*node)->prev->content = NULL;
				if ((*node)->prev->prev)
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
	if (check_syntax(hashtable) != 1)
		return (0);
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!input_starts_with_command((*hashtable)[i]))
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
