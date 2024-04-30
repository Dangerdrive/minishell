/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:22:45 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:23:01 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_bool	is_redir_in(char *c)
{
	if (c && !ft_strcmp(c, LESS_THAN))
		return (true);
	return (false);
}

t_bool	is_redir_out(char *c)
{
	if (c && !ft_strcmp(c, GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_append(char *c)
{
	if (c && !ft_strcmp(c, DOUBLE_GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_heredoc(char *content)
{
	if (content && strcmp(content, DOUBLE_LESS_THAN) == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	is_redir(char *sig)
{
	if (is_redir_in(sig) || is_redir_out(sig)
		|| is_heredoc(sig) || is_append(sig))
		return (true);
	return (false);
}
