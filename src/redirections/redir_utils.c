/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:01:31 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:13:00 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Generates a temporary filename for to create a heredoc temp file.
 *
 * @param heredoc_number The identifier number for the heredoc.
 * @return Pointer to a dynamically allocated string with the filename.
 */
char	*tmp_filename(int heredoc_number)
{
	char	filename[30];
	char	*number_str;

	ft_bzero(filename, 30);
	number_str = ft_itoa(heredoc_number);
	ft_strlcat(filename, ".tmp/heredoc", 30);
	ft_strlcat(filename, number_str, 30);
	free(number_str);
	return (ft_strdup(filename));
}

/**
 * Checks if the last character of a line is a line break.
 *
 * @param line The line to check.
 * @return TRUE if the line ends with a newline character, otherwise FALSE.
 */
t_bool	ends_with_linebreak(char *line)
{
	int	i;

	i = 0;
	while (line[i + 1] != '\0')
		i++;
	if (line[i] == '\n')
		return (TRUE);
	return (FALSE);
}

/**
 * Opens a file for writing and creates it if it doesn't exist.
 *
 * @param filename The name of the file to open or create.
 * @return The file descriptor for the opened file, or -1 on error.
 */
int	get_fd(char *filename)
{
	int	tmp_file_fd;

	tmp_file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_file_fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", filename, strerror(errno));
		free(filename);
		return (-1);
	}
	return (tmp_file_fd);
}

/**
 * Handles the situation when heredoc ends due to reaching an EOF instead of
 * the specified delimiter.
 *
 * @param data Pointer to global data structure.
 * @param eof The expected delimiter for the heredoc.
 */
void	handle_heredoc_ctrl_d(t_global *data, char *eof)
{
	ft_dprintf(STDOUT_FILENO,
		"%sminishell: warning: heredoc delimited by EOF (wanted '%s')%s\n",
		YELLOW, eof, END);
	data->ret = 0;
}
