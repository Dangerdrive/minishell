/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:01:42 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:29:55 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles the writing into a heredoc file by reading lines until the end-of-file
 * (EOF) delimiter is encountered.
 *
 * @param data Pointer to the global data structure.
 * @param heredoc_number Unique identifier for the heredoc.
 * @param hash_number Unused parameter, reserved for future use.
 * @param eof The end-of-file delimiter to stop reading.
 */
void	write_in_heredoc(t_global *data, int heredoc_number,
			int hash_number, char *eof)
{
	int		tmp_file_fd;
	char	*filename;
	char	*line;

	tmp_file_fd = 0;
	(void)hash_number;
	filename = tmp_filename(heredoc_number);
	tmp_file_fd = get_fd(filename);
	line = readline("> ");
	while (line && ft_strncmp(line, eof, (ft_strlen(eof) + 1)))
	{
		expand_heredoc(data, &line);
		ft_putstr_fd(line, tmp_file_fd);
		ft_putchar_fd('\n', tmp_file_fd);
		free(line);
		line = readline("> ");
	}
	if (!line)
		handle_heredoc_ctrl_d(data, eof);
	close(tmp_file_fd);
	free(filename);
	free(line);
}

/**
 * Prints an error message when opening a file descriptor fails during
 *  redirection.
 *
 * @param input_redirect The redirection string causing the error.
 * @return 0, indicating a failure to open the file descriptor.
 */
static int	error_open_fd(char *input_redirect)
{
	ft_dprintf(STDERR_FILENO, "Fail to open heredoc: %s: %s\n",
		&input_redirect[2], strerror(errno));
	return (0);
}

/**
 * Handles input redirection by opening the specified file or heredoc.
 *
 * @param input_redirect The redirection command string.
 * @param i The index or identifier for temporary filenames in heredocs.
 * @return 1 if successful, 0 otherwise.
 */
int	redirect_input(char *input_redirect, int i)
{
	int		fd;
	char	*filename;

	if (input_redirect[1] == '<')
	{
		filename = tmp_filename(i);
		fd = open(filename, O_RDONLY, FD_CLOEXEC);
		if (fd == -1)
		{
			free(filename);
			return (error_open_fd(input_redirect));
		}
		free(filename);
	}
	else
	{
		fd = open(&input_redirect[2], O_RDONLY, FD_CLOEXEC);
		if (fd == -1)
			return (error_open_fd(input_redirect));
	}
	redirect_fd(fd, STDIN_FILENO);
	return (1);
}

/**
 * Handles output redirection by opening or creating a file with appropriate
 *  flags.
 *
 * @param output_redirect The redirection command string.
 * @return 1 if the file is successfully opened/created and redirected,
 *  0 if not.
 */
int	redirect_output(char *output_redirect)
{
	int	fd;

	if (output_redirect[1] == '>')
		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
		return (0);
	}
	else
	{
		redirect_fd(fd, STDOUT_FILENO);
	}
	return (1);
}
