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

char	*ft_strarr_str(char **strarr, const char *str)
{
	int		i;
	int		len;
	char	*result;

	if (!strarr || !str)
		return (NULL);
	len = ft_strarr_len(strarr);
	i = 0;
	while (i < len)
	{
		if (!strarr[i])
			continue;	
		result = ft_strnstr(strarr[i], str, ft_strlen(str));
		if (result)
			return (result);
		i++;
	}
	return (result);
}

