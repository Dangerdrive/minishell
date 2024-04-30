/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:26:01 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:26:02 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	key_matches_target(const char *strarr_elem, const char *tgt)
{
	char	*key;
	int		result;

	key = strndup(strarr_elem, ft_strchr_i(strarr_elem, '='));
	result = ft_strcmp(key, tgt);
	free(key);
	return (result == 0);
}
