#include "../includes/minishell.h"

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

void	check_heredoc(t_tkn **node)
{
	t_tkn	*temp;

	temp = NULL;
	if (strncmp((*node)->content, DOUBLE_LESS_THAN, 2) == 0
		&& (*node)->next && !is_special_token((*node)->next->content))
	{
		(*node)->delimiter = ft_strdup((*node)->next->content);
	}
}

void	update_redir_files_list(char *(*redir)[TABLE_SIZE], char *sig, char *new_arg)
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

void	update_node_after_heredoc(t_tkn **node)
{
	t_tkn	*temp;

	free((*node)->content);
	(*node)->content = NULL;
	if ((*node)->prev)
		temp = (*node)->prev;
	else
	{
		temp = *node;
		*node = (*node)->next;
	}
	temp->next = (*node)->next;
	if (temp && temp->next)
	{
		temp->next->prev = temp;
	}
	if (*node)
	{
		free((*node)->content);
		free(*node);
	}
	if (temp->prev && temp->prev->prev)
		free(temp->prev->prev);
	(*node) = temp;
}

void	update_node_after_redir(t_tkn **node)
{
	t_tkn	*temp;

	temp = NULL;
	free((*node)->content);
	(*node)->content = NULL;
	if ((*node)->prev)
		temp = (*node)->prev;
	else
		temp = *node;
	temp->next = (*node)->next->next;
	if ((*node)->next->next)
		temp->next->prev = temp;
	free((*node)->next->content);
	free((*node)->next);
	free((*node));
	*node = temp;
}

void	parse_redir(t_tkn **node, t_tkn **temp_node)
{
	if (is_heredoc((*node)->content))
		check_heredoc(node);
	if (is_redir((*node)->content) && (*node)->next)
	{
		if (!(*temp_node) && (*node)->prev)
			*temp_node = (*node)->prev;
		else if (!(*temp_node))
			*temp_node = *node;
		if (!(*temp_node)->redir[0])
			init_redir_args(&(*temp_node)->redir);
		update_redir_files_list(&(*temp_node)->redir, (*node)->content, (*node)->next->content);
		if (is_heredoc((*node)->content))
			update_node_after_heredoc(node);
		else
			update_node_after_redir(node);
	}
}
