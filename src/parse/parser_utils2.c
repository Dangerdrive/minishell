/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:36 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:08:18 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the content string ends with an equals sign followed by nothing,
 * which indicates an attempt to export a variable without a value.
 *
 * @param content The string to check.
 * @return True if the string ends with '=', false otherwise.
 */
t_bool	is_export_var(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '=' && !content[i + 1])
			return (true);
		i++;
	}
	return (false);
}

/**
 * Checks if the string contains a variable reference starting with '$'
 * followed by an alphabetic character or an underscore.
 *
 * @param content The string to examine.
 * @return True if a variable reference exists, false otherwise.
 */
t_bool	check_there_is_var(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '$' && (ft_isalpha(content[i + 1])
				|| content[i + 1] == '_'))
			return (true);
		i++;
	}
	return (false);
}

/**
 * Determines if a string contained within quotes is empty, ignoring spaces.
 *
 * @param content The string to check.
 * @param quote The quote character delimiting the string.
 * @return True if the string is empty (excluding spaces), false otherwise.
 */
t_bool	is_empty_str(char *content, char quote)
{
	int	i;

	i = 1;
	while (content[i] && content[i] != quote)
	{
		if (content[i] != 32)
			return (false);
		i++;
	}
	return (true);
}
