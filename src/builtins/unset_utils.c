/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:58:36 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:58:37 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Validates if the provided string is a valid identifier for unset operations.
 * 
 * @param[in] str The string to validate as an identifier.
 * @return TRUE if the identifier is valid (alphanumeric or underscore, not
 *         starting with a digit), FALSE otherwise.
 */
t_bool	unset_identifier_is_valid(char *str)
{
	int		i;

	if (str == NULL)
		return (FALSE);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (FALSE);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/**
 * Checks if the key part of a 'key=value' string matches the specified target.
 * 
 * @param[in] strarr_elem The string element in 'key=value' format.
 * @param[in] tgt The target key to match.
 * @return 1 if the key matches the target, 0 otherwise.
 */
int	key_matches_target(const char *strarr_elem, const char *tgt)
{
	char	*key;
	int		result;

	key = strndup(strarr_elem, ft_strchr_i(strarr_elem, '='));
	result = ft_strcmp(key, tgt);
	free(key);
	return (result == 0);
}
