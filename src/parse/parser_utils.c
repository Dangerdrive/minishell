/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:32 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:07:15 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Checks if a given token represents a pipe.
 *
 * @param token The token to check.
 * @return True if the token is a pipe, false otherwise.
 */
t_bool	is_pipe(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0))
		return (true);
	return (false);
}

/**
 * Determines if a token is a logical AND (&&) or OR (||) operator.
 *
 * @param token The token to check.
 * @return True if the token is either '&&' or '||', false otherwise.
 */
t_bool	is_and_or(char *token)
{
	if (token && (ft_strcmp(token, "&&") == 0
			|| ft_strcmp(token, "||") == 0))
		return (true);
	return (false);
}

/**
 * Checks if a token is either a pipe or a logical operator (AND/OR).
 *
 * @param token The token to check.
 * @return True if the token is a pipe, '&&', or '||', false otherwise.
 */
t_bool	is_pipe_and_or(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0
			|| ft_strcmp(token, "&&") == 0
			|| ft_strcmp(token, "||") == 0))
		return (true);
	return (false);
}

/**
 * Identifies if a token is a special shell token like redirections or control
 * operators.
 *
 * @param token The token to check.
 * @return True if the token is a special shell token, false otherwise.
 */
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

/**
 * Determines if a token and its following token are both special tokens,
 * excluding the case where a pipe is followed by an input redirection.
 *
 * @param node The current token node.
 * @return True if both current and next tokens are special, false otherwise.
 */
t_bool	is_double_special_token(t_tkn *node)
{
	if ((!node->next || (node->next && is_special_token(node->next->content)))
		&& is_special_token(node->content) && node->next
		&& (!is_pipe(node->content) && !is_redir_in(node->next->content)))
		return (true);
	return (false);
}
