/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strarr_dup copy 2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:38:40 by fde-alen          #+#    #+#             */
/*   Updated: 2024/03/28 14:26:04 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strarr_free(char **array, int len)
{
	while (len > 0)
	{
		len--;
		free(array[len]);
	}
	free(array);
}
