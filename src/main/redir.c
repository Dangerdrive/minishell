#include "../../includes/minishell.h"

/**
 * Opens the output file and sets its file descriptor in the data structure.
 *
 * This function determines the correct flags for opening the output file based
 * on whether the 'here_doc' feature is being used. If 'here_doc' is enabled, it
 * opens the file in append mode (O_APPEND), allowing the contents to be added to
 * the end of the file. If 'here_doc' is not enabled, it opens the file in
 * truncate mode (O_TRUNC), which clears the file's existing contents. In both
 * cases, the file is opened for writing (O_WRONLY) and will be created (O_CREAT)
 * if it doesn't already exist, with standard permissions set (0644).
 *
 * If the file cannot be opened, an error message is printed to standard output
 * using `ft_printf`, displaying the corresponding error from `errno` and the
 * file's name.
 *
 * @param[in,out] data A pointer to the t_data structure containing information
 *                     about the pipeline execution, including the heredoc flag
 *                     and file descriptors for input and output files.
 */
static void	handle_heredoc(t_data *data)
{
	int		tmp_fd;
	int		stdin_fd;
	char	*line;

	tmp_fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	stdin_fd = dup(STDIN_FILENO);
	if (tmp_fd == -1)
		cleanup_n_exit(ft_printf_fd(2, "here_doc: %s\n", strerror(errno)), data);
	while (1)
	{
		ft_printf_fd(1, "> ");
		line = get_next_line(stdin_fd);
		if (line == NULL)
			break ;
		if (ft_strlen(data->here_doc) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, data->here_doc, ft_strlen(data->here_doc))) //checar se essa logica faz sentido
		{
			close(stdin_fd);
			break ;
		}
		else
			ft_printf_fd(tmp_fd, line);
		free(line);
	}
	close(tmp_fd);
}

/**
 * Retrieves and opens the input file for the pipex program.
 *
 * This function handles the opening of the input file for the pipex program.
 * If the 'here_doc' flag is set in the t_data structure, it manages the
 * here_doc functionality, creating a temporary file to store the here_doc
 * input. Otherwise, it opens the file specified as the first command line
 * argument. The file descriptor for the opened file is stored in the `input_fd`
 * field of the t_data structure. If the file cannot be opened, an error message
 * is printed, and the program exits after cleanup.
 *
 * @param[in,out] data Pointer to a t_data structure containing the heredoc flag,
 *                     command line arguments, and the input file descriptor to
 * be set.
 */
int	get_input_file(t_global *data, t_tkn *tkn)
{
	if (tkn->here_doc != NULL)
	{
		handle_heredoc(data);
		data->input_fd = open(".heredoc.tmp", O_RDONLY);
		if (data->input_fd == -1)
		{
			ft_printf_fd(2, "here_doc: %s\n", strerror(errno));
			//cleanup_n_exit(ERROR, data);
			return (0);
		}
	}
	else
	{
		data->input_fd = open(data->input, O_RDONLY, 644);
		if (data->input_fd == -1)
		{
			ft_printf_fd(2, "%s: %s\n", data->input, strerror(errno));
			return (0);
		}	
	}
	return (1);
}

/**
 * Opens the output file for writing in the pipex program.
 *
 * This function is responsible for opening the output file for the pipex
 * program. It handles two cases: if the 'here_doc' flag is set in the t_data
 * structure, the file is opened in append mode (O_APPEND), allowing data to be
 * added to the end of the file without altering its existing content. Otherwise,
 * the file is opened in truncate mode (O_TRUNC), which will clear any existing
 * content. The file descriptor for the opened file is stored in `output_fd`
 * field of the t_data structure. If there is an error opening the file, an
 * error message is printed.
 *
 * @param[in,out] data Pointer to a t_data structure containing the heredoc flag,
 *                     command line arguments, and the output file descriptor
 *  to be set.
 */
int	get_output_file(t_global *data)
{
	data->output_fd = open(data->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->output_fd == -1)
	{
		//cleanup_n_exit(ERROR, data);
		ft_printf_fd(2, "%s: %s\n",	data->output, strerror(errno));
		return (0);
	}
}