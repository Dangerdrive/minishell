/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarr_dup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:38:40 by fde-alen          #+#    #+#             */
/*   Updated: 2024/03/28 20:27:49 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Duplicates an array of strings.
 *
 * @param array The array of strings to duplicate.
 * @return The duplicated array of strings, or NULL if the allocation fails.
 */
char	**ft_strarr_dup(char **array)
{
	int		i;
	char	**dup;

	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
		i++;
	dup = ft_calloc(1, (i + 1) * sizeof(char *));
	if (!dup)
		return (NULL);
	i = 0;
	while (array[i])
	{
		dup[i] = ft_strdup(array[i]);
		if (!dup[i])
		{
			ft_strarr_free(dup, i);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}	
