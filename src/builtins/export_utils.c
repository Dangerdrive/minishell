/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:25:36 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:25:37 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_strarr_str_exp_rplc(char **strarr, char *target, char *replcmt)
{
	int		i;
	int		len;
	char	*strarr_key;

	if (!strarr || !target || !replcmt)
		return ;
	len = ft_strarr_len(strarr);
	i = 0;
	while (i < len)
	{
		if (ft_strchr_i(strarr[i], '=') != -1)
			strarr_key = ft_strndup(strarr[i], ft_strchr_i(strarr[i], '='));
		else
			strarr_key = ft_strdup(strarr[i]);
		if (strarr[i] && ft_strcmp(strarr_key, target) == 0)
		{
			free(strarr[i]);
			strarr[i] = ft_strdup(replcmt);
			if (!strarr[i])
				return ;
		}
		free(strarr_key);
		i++;
	}
}

int	ft_strarr_str_exp(char **strarr, const char *target)
{
	int		i;
	char	*strarr_key;

	if (!strarr || !target)
		return (-1);
	i = 0;
	while (strarr[i])
	{
		if (ft_strchr_i(strarr[i], '=') != -1)
			strarr_key = ft_strndup(strarr[i], ft_strchr_i(strarr[i], '='));
		else
			strarr_key = ft_strdup(strarr[i]);
		if (ft_strcmp(strarr_key, target) == 0)
			return (i);
		free(strarr_key);
		i++;
	}
	return (-1);
}
