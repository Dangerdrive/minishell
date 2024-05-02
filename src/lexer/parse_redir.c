#include "../../includes/minishell.h"

void	init_redir_args(char *(*args)[TABLE_SIZE]);
void	check_heredoc(t_tkn **node);
void	update_redir_files_list(char *(*redir)[TABLE_SIZE],
			char *sig, char *new_arg);

static void	free_1st_content_and_delimiter(t_tkn **node)
{
	free((*node)->content);
	(*node)->content = NULL;
	if ((*node)->delimiter)
		free((*node)->delimiter);
}

static void	handle_no_prev_node(t_tkn **node)
{
	t_tkn	*temp;

	free_1st_content_and_delimiter(node);
	if ((*node)->next && (*node)->next->next)
	{
		temp = (*node)->next;
		ft_memdel(*node);
		*node = temp;
		temp = (*node)->next;
		ft_memdel((*node)->content);
		ft_memdel(*node);
		temp->prev = NULL;
	}
	else
	{
		temp = *node;
		ft_memdel((*node)->next->content);
		ft_memdel((*node)->next);
		temp->next = NULL;
	}
	if (temp->next)
		temp->next->prev = temp;
	*node = temp;
}

static void	handle_prev_node(t_tkn **node)
{
	t_tkn	*temp;

	temp = (*node)->prev;
	if ((*node)->delimiter && temp->delimiter)
		temp->delimiter = ft_strdup((*node)->delimiter);
	if ((*node)->delimiter)
		free((*node)->delimiter);
	*node = (*node)->next;
	free((*node)->prev->content);
	free((*node)->prev);
	free((*node)->content);
	temp->next = (*node)->next;
	free(*node);
	if (temp->next)
		temp->next->prev = temp;
	*node = temp;
}

static void	update_node_after_redir(t_tkn **node)
{
	if ((*node)->prev)
		handle_prev_node(node);
	else
		handle_no_prev_node(node);
}

void	parse_redir(t_tkn **node, t_tkn **head)
{
	int		i;
	t_tkn	*temp;

	i = 0;
	temp = *node;
	if (is_heredoc((*node)->content))
		check_heredoc(node);
	while (!ft_strcmp((*head)->content, temp->content)
		&& is_redir(temp->content) && temp->next && temp->next->next)
	{
		*head = temp->next->next;
		temp = *head;
	}
	if (!(*head)->redir[0])
		init_redir_args(&(*head)->redir);
	update_redir_files_list(&(*head)->redir,
		(*node)->content, (*node)->next->content);
	if (is_heredoc((*node)->content))
		update_node_after_redir(node);
	else
		update_node_after_redir(node);
}
