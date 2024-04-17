#include "../../includes/minishell.h"

// /**
//  * Opens the output file and sets its file descriptor in the data structure.
//  *
//  * This function determines the correct flags for opening the output file based
//  * on whether the 'here_doc' feature is being used. If 'here_doc' is enabled, it
//  * opens the file in append mode (O_APPEND), allowing the contents to be added to
//  * the end of the file. If 'here_doc' is not enabled, it opens the file in
//  * truncate mode (O_TRUNC), which clears the file's existing contents. In both
//  * cases, the file is opened for writing (O_WRONLY) and will be created (O_CREAT)
//  * if it doesn't already exist, with standard permissions set (0644).
//  *
//  * If the file cannot be opened, an error message is printed to standard output
//  * using `ft_printf`, displaying the corresponding error from `errno` and the
//  * file's name.
//  *
//  * @param[in,out] data A pointer to the t_data structure containing information
//  *                     about the pipeline execution, including the heredoc flag
//  *                     and file descriptors for input and output files.
//  */
// int	handle_heredoc(t_tkn *node, char *)
// {
// 	int		tmp_fd;
// 	int		stdin_fd;
// 	char	*line;


// 	tmp_fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	stdin_fd = dup(STDIN_FILENO);
// 	if (tmp_fd == -1)
// 	{
// 		ft_dprintf(2, "minishell: here_doc: %s\n", strerror(errno));
// 		return (1);
// 	}
// 	while (1)
// 	{
// 		ft_dprintf(1, "> ");
// 		line = get_next_line(stdin_fd);
// 		if (line == NULL)
// 			break ;
// 		if (ft_strlen(node->here_doc) + 1 == ft_strlen(line)
// 			&& !ft_strncmp(line, node->here_doc, ft_strlen(node->here_doc))) //checar se essa logica faz sentido
// 		{
// 			close(stdin_fd);
// 			break ;
// 		}
// 		else
// 			ft_dprintf(tmp_fd, line);
// 		free(line);
// 	}
// 	close(tmp_fd);
// 	return (0);
// }


//como fica a questão do nome do arquivo temporário? precisa salvar?
int handle_heredoc(t_tkn *node, char *delimiter, char *index)
{
	int		tmp_fd;
	char	*line;
	char	*heredoc_filename;
	char	*heredoc_temp = ".heredoc_";

	heredoc_filename = malloc(ft_strlen(heredoc_temp) + ft_strlen(index) + 1);
	if (!heredoc_filename)
	{
		perror("Memory allocation failed");
		return -1;
	}
	strcpy(heredoc_filename, heredoc_temp);
	strcat(heredoc_filename, index);
	tmp_fd = open(heredoc_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
	{
		perror("Error creating temp file for heredoc");
		free(heredoc_filename);
		return -1;
	}
	while (1)
	{
		ft_dprintf(1, "> ");
		line = get_next_line(STDIN_FILENO);
		if (line == NULL) {
			free(heredoc_filename);
			close(tmp_fd);
			return -1;
		}
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break;
		}
		ft_dprintf(tmp_fd, "%s\n", line);
		free(line);
	}
	close(tmp_fd);
	node->input_fd = open(heredoc_filename, O_RDONLY);
	if (node->input_fd == -1)
	{
		perror("Error opening heredoc temp file for reading");
		free(heredoc_filename);
		return -1;
	}
	free(heredoc_filename);
	return 0;
}


// int handle_heredoc(t_tkn *node, char *delimiter, char *index)
// {
// 	int  	tmp_fd;
// 	int		stdin_fd;
// 	char	*line;
// 	char	*heredoc_tmp;
 
// 	heredoc_tmp = ft_strjoin(delimiter, index);
// 	free(index);
// 	tmp_fd = open(heredoc_tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	stdin_fd = dup(STDIN_FILENO);
// 	if (tmp_fd == -1)
// 	{
// 		perror("Error creating temp file for heredoc");
// 		return -1;
// 	}
// 	while (1)
// 	{
// 		ft_dprintf(1, "> ");
// 		line = get_next_line(stdin_fd);
// 		if (line == NULL)
// 			break;
// 		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
// 		{
// 			close(stdin_fd);
// 			free(line);
// 			break;
// 		}
// 		else
// 			ft_dprintf(tmp_fd, "%s", line);
// 		free(line);
// 	}
// 	close(tmp_fd);
// 	close(stdin_fd);
// 	node->input_fd = open(".heredoc.tmp", O_RDONLY);
// 	if (node->input_fd == -1)
// 	{
// 		perror("Error opening heredoc temp file for reading");
// 		return -1;
// 	}
// 	return 0;
// }

// /**
//  * Retrieves and opens the input file for the pipex program.
//  *
//  * This function handles the opening of the input file for the pipex program.
//  * If the 'here_doc' flag is set in the t_data structure, it manages the
//  * here_doc functionality, creating a temporary file to store the here_doc
//  * input. Otherwise, it opens the file specified as the first command line
//  * argument. The file descriptor for the opened file is stored in the `input_fd`
//  * field of the t_data structure. If the file cannot be opened, an error message
//  * is printed, and the program exits after cleanup.
//  *
//  * @param[in,out] data Pointer to a t_data structure containing the heredoc flag,
//  *	         command line arguments, and the input file descriptor to
//  * be set.
//  */
// int	get_input_file(t_global *data, t_tkn *tkn)
// {
// 	if (tkn->here_doc != NULL)
// 	{
// 		handle_heredoc(data);
// 		data->input_fd = open(".heredoc.tmp", O_RDONLY);
// 		if (data->input_fd == -1)
// 		{
// 			ft_dprintf(2, "here_doc: %s\n", strerror(errno));
// 			//cleanup_n_exit(ERROR, data);
// 			return (0);
// 		}
// 	}
// 	else
// 	{
// 		data->input_fd = open(data->input, O_RDONLY, 644);
// 		if (data->input_fd == -1)
// 		{
// 			ft_dprintf(2, "%s: %s\n", data->input, strerror(errno));
// 			return (0);
// 		}	
// 	}
// 	return (1);
// }

// /**
//  * Opens the output file for writing in the pipex program.
//  *
//  * This function is responsible for opening the output file for the pipex
//  * program. It handles two cases: if the 'here_doc' flag is set in the t_data
//  * structure, the file is opened in append mode (O_APPEND), allowing data to be
//  * added to the end of the file without altering its existing content. Otherwise,
//  * the file is opened in truncate mode (O_TRUNC), which will clear any existing
//  * content. The file descriptor for the opened file is stored in `output_fd`
//  * field of the t_data structure. If there is an error opening the file, an
//  * error message is printed.
//  *
//  * @param[in,out] data Pointer to a t_data structure containing the heredoc flag,
//  *                     command line arguments, and the output file descriptor
//  *  to be set.
//  */
// int	get_output_file(t_global *data)
// {
// 	data->output_fd = open(data->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (data->output_fd == -1)
// 	{
// 		//cleanup_n_exit(ERROR, data);
// 		ft_dprintf(2, "%s: %s\n",	data->output, strerror(errno));
// 		return (0);
// 	}
// }

// int parse_redirections(t_global *data, t_tkn *node)
// {
// 	int i;

// 	i = 0;
// 	while (node->redir && node->redir[i])
// 	{
// 		if (node->redir[i][0] == '>')
// 		{
// 			if (node->redir[i][1] == '>')
// 				node->output_fd = open(&(node->redir[i][2]), O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			else
// 				node->output_fd = open(node->redir[i][2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			data->output = node->redir[i];
// 		}
// 		if (node->redir[i][0] == '<')
// 		{
// 			if (node->redir[i][1] == '<')
// 				node->input_fd = open(node->redir[i], O_WRONLY | O_CREAT, 644);//mudar para uma pasta temp
// 			else
// 				node->input_fd = open(node->redir[i], O_RDONLY, 644);
// 			data->input = node->redir[i];
// 		}
// 		i++;
// 	}
// }

// int	redir_input(t_tkn *node, char *input, int index)
// {
// 	int		fd;
// 	char 	*file;

// 	if (input[1] == '<')
// 	{
// 		file = ft_strjoin(input, index);
// 		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);		
// 	}
// 	else
// 	{
// 		file = ft_strdup(input);
// 		fd = open(file, O_RDONLY);
// 	}	
// 	if (fd == -1) 
// 		return -1;
// 	close(node->input_fd);
// 	node->input_fd = fd;
// 	node->input = ft_strdup(input);//
// 	free(file);//
// 	return (0);
// }

int redir_input(t_tkn *node, char *input, int index)
{
    int     fd;
    char    *h_index;

    if (input[1] == '<')
	{
        h_index = ft_itoa(index);
        return handle_heredoc(node, input, h_index);
    } 
	else
	{
        fd = open(input, O_RDONLY);
        if (fd == -1)
		{
            perror("Error opening file");
            return -1;
        }
        close(node->input_fd);
        node->input_fd = fd;
        node->input = input;
    }
    return 0;
}

int	redir_output(t_tkn *node, char *output)
{
	int fd;

	if (output[1] == '>')
		fd = open(output, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) 
		return 1;
	close(node->output_fd);
	node->output_fd = fd;
	node->output = output;
	return (0);
}

int parse_redirections(t_tkn *node)
{
	int i;

	i = 0;
	while (node->redir[i])
	{
		if (node->redir[i][0] == '>')
        {
			if(redir_output(node, &(node->redir[i][2])))
				return 1;
        }
		else if (node->redir[i][0] == '<')
		{
        	if (redir_input(node, &(node->redir[i][2]), i))
				return 1;
        }
		i++;
	}
	return 0;
}

// int parse_redirections(t_global *data, t_tkn *node)
// {
// 	int i;
// 	int fd;

// 	i = 0;
// 	while (node->redir && node->redir[i])
// 	{
// 		if (node->redir[i][0] == '>')
// 		{
// 			redir_output(data, node, &(node->redir[i][2]));
// 			// if (node->redir[i][1] == '>')
// 			// 	fd = open(&(node->redir[i][2]), O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			// else
// 			// 	fd = open(&(node->redir[i][1]), O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			// if (fd == -1) 
// 			// 	return -1;
// 			// close(node->output_fd);
// 			// node->output_fd = fd;
// 			// data->output = node->redir[i];
// 		}
// 		else if (node->redir[i][0] == '<')
// 		{
// 			redir_input(data, node);
// 			// if (node->redir[i][1] == '<')
// 			// 	fd = open(&(node->redir[i][2]), O_WRONLY | O_CREAT, 0644);
// 			// else
// 			// 	fd = open(&(node->redir[i][1]), O_RDONLY);
// 			// if (fd == -1) 
// 			// 	return -1;
// 			// close(node->input_fd);
// 			// node->input_fd = fd;
// 			// data->input = node->redir[i];
// 		}
// 		i++;
// 	}
// 	return 0;
// }


// expansões no redirect