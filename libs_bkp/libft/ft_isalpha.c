/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 18:59:21 by fde-alen          #+#    #+#             */
/*   Updated: 2024/04/01 16:20:46 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Checks if the given character 'c' is an alphabetic character.
** An alphabetic character is any from 'a' to 'z' or from 'A' to 'Z'.
**
** Parameters:
** - c: The character to check.
**
** Returns:
** - Non-zero value if 'c' is an alphabetic character.
** - Zero if 'c' is not an alphabetic character.
*/
int	ft_isalpha(int c)
{
	return (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')));
}
