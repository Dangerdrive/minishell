/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:11 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:03:16 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the specified command path refers to a folder.
 *
 * @param command The file path to check.
 * @return 1 if the path is a folder, 0 otherwise.
 */
int	is_folder(char *command)
{
	struct stat	statbuf;

	if (stat(command, &statbuf) != 0)
		return (0);
	if (S_ISDIR(statbuf.st_mode))
	{
		if (*command == '.')
			command++;
		if (*command == '.')
			command++;
		if (*command == '/')
			return (TRUE);
	}
	return (FALSE);
}

/**
 * Terminates the external process, clearing history and closing file
 *  descriptors.
 *
 * @param exit_status The status code to exit the process with.
 */
void	external_exit(int exit_status)
{
	rl_clear_history();
	close_all_fds();
	exit(exit_status);
}
