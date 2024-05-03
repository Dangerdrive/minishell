/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:58:25 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:58:26 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Replaces an element in a string array matching the target with a replacement.
 * 
 * @param[in, out] strarr Array of strings containing key=value pairs.
 * @param[in] target The key to match for replacement.
 * @param[in] replcmt The string to replace the matching element.
 */
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

/**
 * Searches for a string key in a string array and returns its index.
 * 
 * @param[in] strarr Array of strings containing key=value pairs.
 * @param[in] target The key to search for.
 * @return Index of the matching key, or -1 if not found.
 */
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
