/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:22:24 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 18:33:22 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_redir_args(char *(*args)[TABLE_SIZE]);
void	check_heredoc(t_tkn **node);
void	update_redir_files_list(char *(*redir)[TABLE_SIZE],
			char *sig, char *new_arg);

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
		temp->next->prev = temp;
	if (*node)
	{
		free((*node)->content);
		(*node)->content = NULL;
		free(*node);
	}
	if (temp->prev && temp->prev->prev)
		free(temp->prev->prev);
	(*node) = temp;
}

// void	update_node_after_redir(t_tkn **node)
// {
// 	t_tkn	*temp;

// 	temp = NULL;
// 	free((*node)->content);
// 	(*node)->content = NULL;
// 	if ((*node)->prev)
// 	{
// 		temp = (*node)->prev;
// 		*node = (*node)->next;
// 		free((*node)->prev);
// 		free((*node)->content);
// 		temp->next = (*node)->next;
// 		free(*node);
// 	}
// 	else if (!(*node)->prev && (*node)->next && (*node)->next->next)
// 	{
// 		temp = (*node)->next;
// 		free(*node);
// 		*node = temp;
// 		temp = (*node)->next;
// 		free((*node)->content);
// 		free(*node);
// 		temp->prev = NULL;
// 	}
// 	else
// 	{
// 		temp = *node;
// 		free((*node)->next->content);
// 		(*node)->next->content = NULL;
// 		free((*node)->next);
// 		temp->next = NULL;
// 	}
// 	if (temp->next)
// 		temp->next->prev = temp;
// 	*node = temp;
// }

static void	handle_no_prev_node(t_tkn **node)
{
	t_tkn	*temp;

	if ((*node)->next && (*node)->next->next)
	{
		temp = (*node)->next;
		free(*node);
		*node = temp;
		temp = (*node)->next;
		free((*node)->content);
		free(*node);
		temp->prev = NULL;
	}
	else
	{
		temp = *node;
		free((*node)->next->content);
		(*node)->next->content = NULL;
		free((*node)->next);
		temp->next = NULL;
	}
	if (temp->next)
		temp->next->prev = temp;
	free((*node)->content);
	(*node)->content = NULL;
	*node = temp;
}

static void	handle_prev_node(t_tkn **node)
{
	t_tkn	*temp;

	temp = (*node)->prev;
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
	if (is_heredoc((*node)->content))
		check_heredoc(node);
	if (!ft_strcmp((*head)->content, (*node)->content)
		&& (*node)->next && (*node)->next->next)
		*head = (*node)->next->next;
	if (!(*head)->redir[0])
		init_redir_args(&(*head)->redir);
	update_redir_files_list(&(*head)->redir,
		(*node)->content, (*node)->next->content);
	if (is_heredoc((*node)->content))
		update_node_after_heredoc(node);
	else
		update_node_after_redir(node);
}
