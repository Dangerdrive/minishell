/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:01:27 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:11:40 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Saves the original file descriptor for standard input if not already saved.
 *
 * @param original_fds Array where original file descriptors are stored.
 */
void	save_original_fd_in(int original_fds[2])
{
	if (original_fds[IN] == -1)
		original_fds[IN] = dup(STDIN_FILENO);
}

/**
 * Saves the original file descriptor for standard output if not already saved.
 *
 * @param original_fds Array where original file descriptors are stored.
 */
void	save_original_fd_out(int original_fds[2])
{
	if (original_fds[OUT] == -1)
		original_fds[OUT] = dup(STDOUT_FILENO);
}

/**
 * Handles input redirection based on the type of redirection specified.
 *
 * @param redir The redirection command.
 * @param here_num Index used to specify heredoc number.
 * @param original_fds Array of original file descriptors.
 * @return 0 if redirection is successful, 1 otherwise.
 */
int	handle_input_redirect(char *redir, int here_num, int original_fds[2])
{
	save_original_fd_in(original_fds);
	if (redirect_input(redir, here_num) == 0)
	{
		redirect_fd(original_fds[IN], STDIN_FILENO);
		return (0);
	}
	return (1);
}

/**
 * Handles output redirection based on the redirection command.
 *
 * @param redir The redirection command.
 * @param original_fds Array of original file descriptors.
 * @return 0 if redirection is successful, 1 otherwise.
 */
int	handle_output_redirect(char *redir, int original_fds[2])
{
	save_original_fd_out(original_fds);
	if (redirect_output(redir) == 0)
	{
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
		return (0);
	}
	return (1);
}

/**
 * Manages all redirections found in the command hashtable.
 *
 * @param data Pointer to the global data structure containing redirection data.
 * @param ori_fds Array where original file descriptors are saved.
 * @return 1 if all redirections are handled successfully, 0 on failure.
 */
int	handle_redirects(t_global *data, int ori_fds[2])
{
	int	i;

	i = 0;
	ori_fds[IN] = -1;
	ori_fds[OUT] = -1;
	while (data->hashtable[0]->redir[i])
	{
		if (data->hashtable[0]->redir[i][0] == '<')
		{
			if (!handle_input_redirect(data->hashtable[0]->redir[i],
					i, ori_fds))
				return (0);
		}
		if (data->hashtable[0]->redir[i][0] == '>')
		{
			if (!handle_output_redirect(data->hashtable[0]->redir[i], ori_fds))
				return (0);
		}
		i++;
	}
	return (1);
}
