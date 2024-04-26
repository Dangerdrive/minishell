/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarr_sort.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:38:40 by fde-alen          #+#    #+#             */
/*   Updated: 2024/03/28 13:23:23 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Sorts an array of strings in lexicographical order.
 */
char **ft_strarr_sort(char **strarr)
{
	int		ordered;
	int		i;
	int		strarr_len;
	char	*tmp;
	char	**sorted_strarr;

	strarr_len = ft_strarr_len(strarr);
	sorted_strarr = ft_strarr_dup(strarr);
	ordered = 0;
	while (!ordered)
	{
		ordered = 1;
		i = 0;
		while (i < strarr_len - 1) {
			if (ft_strcmp(sorted_strarr[i], sorted_strarr[i + 1]) > 0)
			{
				tmp = sorted_strarr[i];
				sorted_strarr[i] = sorted_strarr[i + 1];
				sorted_strarr[i + 1] = tmp;
				ordered = 0;
			}
			i++;
		}
		strarr_len--;
	}
	return (sorted_strarr);
}
