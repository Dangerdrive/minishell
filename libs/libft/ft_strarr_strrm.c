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
void	shift_elements_down(char ***strarr, int start_index, int array_length)
{
	while (start_index + 1 < array_length)
	{
		(*strarr)[start_index] = (*strarr)[start_index + 1];
		start_index++;
	}
}

char	**ft_strarr_strrm(char ***strarr, const char *str)
{
	int	i;
	int	len;
	int	found;

	len = ft_strarr_len(*strarr);
	i = 0;
	found = 0;
	while (i < len)
	{
		if (!found && ft_strncmp((*strarr)[i], str, ft_strlen(str)) == 0)
		{
			found = 1;
			/* Uncomment if memory deallocation is needed
			free((*strarr)[i]); */
		}
		if (found)
			shift_elements_down(strarr, i, len);
		else
			i++;
	}
	if (found)
		(*strarr)[len - 1] = NULL;
	return (ft_strarr_ndup(*strarr, len - 1));
}
