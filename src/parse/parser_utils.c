/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:32 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 22:00:33 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_bool	is_pipe(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0))
		return (true);
	return (false);
}

t_bool	is_and_or(char *token)
{
	if (token && (ft_strcmp(token, "&&") == 0
			|| ft_strcmp(token, "||") == 0))
		return (true);
	return (false);
}

t_bool	is_pipe_and_or(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0
			|| ft_strcmp(token, "&&") == 0
			|| ft_strcmp(token, "||") == 0))
		return (true);
	return (false);
}

t_bool	is_special_token(char *token)
{
	if (token
		&& ((ft_strncmp(token, PIPE, 1) == 0)
			|| (ft_strncmp(token, AMPERSAND, 1) == 0)
			|| (ft_strncmp(token, GREATER_THAN, 1) == 0)
			|| (ft_strncmp(token, LESS_THAN, 1) == 0)
			|| (ft_strncmp(token, "||", 2) == 0)
			|| (ft_strncmp(token, "&&", 2) == 0)
			|| (ft_strncmp(token, DOUBLE_GREATER_THAN, 2) == 0)
			|| (ft_strncmp(token, DOUBLE_LESS_THAN, 2) == 0)))
		return (true);
	else
		return (false);
}

t_bool	is_double_special_token(t_tkn *node)
{
	if ((!node->next || (node->next && is_special_token(node->next->content)))
		&& is_special_token(node->content) && node->next
		&& (!is_pipe(node->content) && !is_redir_in(node->next->content)))
		return (true);
	return (false);
}
