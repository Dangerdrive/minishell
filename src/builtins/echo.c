/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:58:10 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:58:11 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Implements the echo command with an option to suppress trailing newline.
 * 
 * @param[in] args The arguments passed to the echo command.
 * @param[in] args_len The number of arguments.
 * @return Always returns 0 to indicate successful execution.
 */
int	ft_echo(char **args, int args_len)
{
	int		i;
	int		n_option;

	i = 1;
	n_option = 0;
	if (args_len > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (args[i])
		{
			ft_dprintf(STDOUT_FILENO, args[i]);
			if (args[i + 1] && args[i][0] != '\0')
				ft_dprintf(STDOUT_FILENO, " ");
			i++;
		}
	}
	if (n_option == 0)
		ft_dprintf(STDOUT_FILENO, "\n");
	return (0);
}
