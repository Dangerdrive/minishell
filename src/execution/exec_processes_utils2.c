/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_processes_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:58:59 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:59:00 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_heredocs(t_global *data)
{
	int	i;
	int	j;

	j = 0;
	while (data->hashtable[j])
	{
		i = 0;
		while (data->hashtable[j]->redir[i])
		{
			if (ft_strncmp(data->hashtable[j]->redir[i], "<<", 2) == 0)
				write_in_heredoc(data, i, j, &data->hashtable[j]->redir[i][2]);
			i++;
		}
		j++;
	}
}

/**
 * Saves the original file descriptors for STDIN and STDOUT.
 * 
 * @param[in, out] original_fds Array to store the file descriptors.
 */
void	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
}
