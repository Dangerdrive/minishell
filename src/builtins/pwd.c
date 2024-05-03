/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:58:32 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:58:33 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Prints the current working directory to standard output.
 * 
 * @return 0 if successful, 1 if an error occurred while retrieving the 
 * directory.
 */
int	ft_pwd(void)
{
	char	*cwd;

	cwd = malloc(4096);
	if (getcwd(cwd, 4096))
	{
		ft_printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		free(cwd);
		return (1);
	}
}
