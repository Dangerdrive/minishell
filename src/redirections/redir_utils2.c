/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:01:35 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:21:01 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Closes all file descriptors above the standard error output to free resources.
 */
void	close_extra_fds(void)
{
	int	last_open_fd;

	last_open_fd = open("/tmp/last_fd", O_RDWR | O_CREAT, 0666);
	if (last_open_fd == -1)
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
			"/tmp/last_fd", strerror(errno));
	while (last_open_fd > STDERR_FILENO)
	{
		close(last_open_fd);
		last_open_fd--;
	}
}

/**
 * Closes all standard file descriptors (stdin, stdout, stderr) and any extra
 * open file descriptors to prevent leaks.
 */
void	close_all_fds(void)
{
	close_extra_fds();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

/**
 * Redirects a file descriptor to another file descriptor and closes
 *  the original.
 *
 * @param fd_to_redirect The file descriptor to redirect.
 * @param fd_location The target file descriptor number.
 */
void	redirect_fd(int fd_to_redirect, int fd_location)
{
	dup2(fd_to_redirect, fd_location);
	close(fd_to_redirect);
}

/**
 * Restores the original standard input and output file descriptors
 *  from backups.
 *
 * @param original_fds Array containing the original file descriptors for stdin
 * and stdout.
 */
void	redirect_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
		redirect_fd(fd_in, STDIN_FILENO);
	if (fd_out != STDOUT_FILENO)
		redirect_fd(fd_out, STDOUT_FILENO);
}

/**
 * Restores the original standard input and output file descriptors from backups.
 *
 * @param original_fds Array containing the original file descriptors for stdin
 * and stdout.
 */
void	restore_original_fds(int original_fds[2])
{
	if (original_fds[IN] != -1)
		redirect_fd(original_fds[IN], STDIN_FILENO);
	if (original_fds[OUT] != -1)
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
}
