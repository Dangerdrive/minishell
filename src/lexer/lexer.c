#include "../includes/minishell.h"

bool	input_starts_with_command(t_tkn	*node, int i)
{
	if ((i == 0 && !node->prev && !is_redir(node->content) && ft_strcmp(node->type, COMMAND))
		|| (i > 0 && node->prev && is_pipe(node->prev->content)
			&& ft_strcmp(node->type, COMMAND)))
		return (false);
	return (true);
}

int	check_valid_input(t_tkn **node, int i)
{
	if (!input_starts_with_command((*node), i))
	{
		printf("%s: command not found\n", (*node)->content);
		return (0);
	}
	if ((is_double_special_token((*node)) && !(*node)->delimiter)
		|| (!(*node)->next && !ft_strcmp((*node)->type, SPECIAL_CHAR))
		|| is_and_or((*node)->content))
	{
		printf("Syntax error.\n");
		return (0);
	}
	return (1);
}

void	init_redir_args(char *(*args)[TABLE_SIZE])
{
	int	i;

	i = 0;
	while (i < TABLE_SIZE)
	{
		(*args)[i] = NULL;
		i++;
	}
	return ;
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
	t_tkn	*temp;

	temp = *node;
	while (*node)
	{
		if ((ft_strcmp((*node)->type, SPECIAL_CHAR)) && (*node)->prev && is_redir((*node)->prev->content))
		{
			if (!temp_node && (*node)->prev->prev)
				temp_node = (*node)->prev->prev;
			else if (!temp_node)
				temp_node = (*node)->prev;
			init_redir_args(&temp_node->redir);
			update_redir_files_list(&temp_node->redir, (*node)->content, (*node)->prev->content);
			update_node_after_redir(node);
		}
		*node = (*node)->next;
	}
	*node = temp;
}

void	remove_pipe(t_tkn **node, int i)
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
			if (!check_valid_input(&(*hashtable)[i], i))
			{
				(*hashtable)[i] = temp;
				return (0);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		remove_pipe(&(*hashtable)[i], i);
		check_redirects(&(*hashtable)[i]);
		i++;
	}
	return (1);
}
