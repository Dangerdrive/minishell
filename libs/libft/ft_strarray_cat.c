/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarray_concat.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:38:40 by fde-alen          #+#    #+#             */
/*   Updated: 2024/03/25 16:52:05 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
*/
char	**ft_strarraycat(char **tab2, char **tab1)
{
	int		i;
	int		j;
	int		tab2_len;
	int		tab1_len;
	char	**new;

	tab2_len = ft_strarray_len(tab2);
	tab1_len = ft_strarray_len(tab1);
	new = (char **)malloc(sizeof(char *) * (tab1_len + tab2_len + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (tab1 && tab1[i])
	{
		new[i] = ft_strdup(tab1[i]);
		i++;
	}
	j = 0;
	while (tab2 && tab2[j])
	{
		new[i + j] = ft_strdup(tab2[j]);
		j++;
	}
	new[i + j] = NULL;
	return (new);
}
