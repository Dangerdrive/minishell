/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarr_stradd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 20:52:11 by fde-alen          #+#    #+#             */
/*   Updated: 2024/04/09 12:40:26 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char **ft_strarr_dup_extend(char **array, int len, const char *new_str)
{
	char	**new_array;
	int		i;

	new_array = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			ft_strarr_free(new_array, i);
			return (NULL);
		}
		i++;
	}
	new_array[i] = ft_strdup(new_str);
	new_array[i + 1] = NULL;
	return (new_array);
}

// Function to add a string to a string array.
void	ft_strarr_stradd(char ***strarr, const char *str)
{
	int		len;
	char 	**new_arr;

	if (!str)
		return ;
	len = ft_strarr_len(*strarr);
	new_arr = ft_strarr_dup_extend(*strarr, len, str);
	if (!new_arr)
		return ;
	ft_strarr_free(*strarr, len);
	*strarr = new_arr;
}
