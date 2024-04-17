#include "../../includes/minishell.h"

int handle_heredoc(t_tkn *node, char *delimiter, char *index)
{
	int		tmp_fd;
	char	*line;
	char	*heredoc_filename;
	char	*heredoc_temp = ".heredoc_";


//atualizar node->input
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

// int redir_input(t_tkn *node, char *input, int index)
// {
//     int     fd;
//     char    *h_index;

//     if (input[1] == '<')
// 	{
//         h_index = ft_itoa(index);
//         return handle_heredoc(node, input, h_index);
//     } 
// 	else
// 	{
//         fd = open(input, O_RDONLY);
//         if (fd == -1)
// 		{
//             perror("Error opening file");
//             return -1;
//         }
//         //close(node->input_fd);
//         node->input_fd = fd;
//         node->input = input;
//     }
//     return 0;
// }

// int	redir_output(t_tkn *node, char *output)
// {
// 	int fd;

// 	if (output[1] == '>')
// 		fd = open(output, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1) 
// 		return 1;
// 	//close(node->output_fd);
// 	node->output_fd = fd;
// 	node->output = output;
// 	return (0);
// }

// int redirect_io(t_tkn *node)
// {
//     // Simplify input redirection
//     if (node->input_fd != -1)
//     {
//         if (dup2(node->input_fd, STDIN_FILENO) == -1)
//         {
//             perror("Failed to redirect standard input");
//             return 1;
//         }
//         close(node->input_fd);  // Good to close after dup2
//         node->input_fd = -1;    // Reset fd after closing
//     }

//     // Simplify output redirection
//     if (node->output_fd != -1)
//     {
//         if (dup2(node->output_fd, STDOUT_FILENO) == -1)
//         {
//             perror("Failed to redirect standard output");
//             return 1;
//         }
//         close(node->output_fd);  // Good to close after dup2
//         node->output_fd = -1;    // Reset fd after closing
//     }

//     return 0;
// }

// int parse_redirections(t_tkn *node)
// {
//     int i = 0;

//     while (node->redir[i])
//     {
//         if (node->redir[i][0] == '>')
//         {
//             if(redir_output(node, &(node->redir[i][2])))
//                 return 1;
//         }
//         else if (node->redir[i][0] == '<')
//         {
//             if (redir_input(node, &(node->redir[i][2]), i))
//                 return 1;
//         }
//         i++;
//     }

//     // Assuming redirection setup handles file opening
//     if (redirect_io(node))
//         return 1;

//     return 0;
// }

int handle_output_redirection(t_tkn *node, char *redirection) {
    int fd = open(redirection, redirection[1] == '>' ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open output file");
        return 1;
    }
    if (node->output_fd != -1) close(node->output_fd);
    node->output_fd = fd;
    node->output = redirection;
    return 0;
}

int handle_input_redirection(t_tkn *node, char *redirection, int index) {
    int fd;
    char *h_index;

    if (redirection[1] == '<') {
        h_index = ft_itoa(index);
        int result = handle_heredoc(node, redirection, h_index);
        free(h_index);
        return result;
    } else {
        fd = open(redirection, O_RDONLY);
        if (fd == -1) {
            perror("Error opening input file");
            return 1;
        }
        if (node->input_fd != -1) close(node->input_fd);
        node->input_fd = fd;
        node->input = redirection;
    }
    return 0;
}

int apply_redirections(t_tkn *node) {
    if (node->input_fd != -1 && dup2(node->input_fd, STDIN_FILENO) == -1) {
        perror("Failed to redirect standard input");
        return 1;
    }
    if (node->output_fd != -1 && dup2(node->output_fd, STDOUT_FILENO) == -1) {
        perror("Failed to redirect standard output");
        return 1;
    }
    return 0;
}

int parse_redirections(t_tkn *node) {
    int i = 0;

    while (node->redir[i]) {
        if (node->redir[i][0] == '>') {
            if (handle_output_redirection(node, &(node->redir[i][2])))
                return 1;
        } else if (node->redir[i][0] == '<') {
            if (handle_input_redirection(node, &(node->redir[i][2]), i))
                return 1;
        }
        i++;
    }
    return apply_redirections(node);
}

// expansões no redirect