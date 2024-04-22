#include "../../includes/minishell.h"

static char	*tmp_filename(int heredoc_number)
{
	char	filename[30];
	char	*number_str;

	ft_bzero(filename, 30);
	number_str = ft_itoa(heredoc_number);
	ft_strlcat(filename, "/tmp/heredoc", 30);
	ft_strlcat(filename, number_str, 30);
	free(number_str);
	return (ft_strdup(filename));
}

void	redirect_heredoc(char *command, int heredoc_number)
{
	char	*filename;
	int		tmp_file_fd;
	char	*h_doc_pos;

	filename = tmp_filename(heredoc_number);
	tmp_file_fd = open(filename, O_RDONLY);
	if (tmp_file_fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", filename, strerror(errno));
		//print_perror_msg("open", filename);
		free(filename);
		return ;
	}
	free(filename);
	redirect_fd(tmp_file_fd, STDIN_FILENO);
	// h_doc_pos = get_redirect_position(command, heredoc_number);
	//move_one_forward(h_doc_pos);
	// ft_memmove(h_doc_pos, h_doc_pos + 1, ft_strlen(h_doc_pos + 1) + 1);
}









//-----------------------input-------------------------------|



// int	is_name_delimeter(char c)
// {
// 	if (c == ' ' || c == '>' || c == '<' || c == '|' || c == '\t')
// 		return (1);
// 	return (0);
// }



// static int	get_name_end(char *name)
// {
// 	int	end;

// 	end = 0;
// 	while (name[end] && !is_name_delimeter(name[end]))
// 	{
// 		if (name[end] == '\'')
// 		{
// 			move_one_forward(&name[end]);
// 			while (name[end] && name[end] != '\'')
// 				end++;
// 			move_one_forward(&name[end]);
// 		}
// 		else if (name[end] == '"')
// 		{
// 			move_one_forward(&name[end]);
// 			while (name[end] && name[end] != '"')
// 				end++;
// 			move_one_forward(&name[end]);
// 		}
// 		else if (name[end] && !is_name_delimeter(name[end]))
// 			end++;
// 	}
// 	return (end);
// }

// char	*get_label_name(char *redirect_str)
// {
// 	int		name_start;
// 	int		name_end;
// 	char	*name;
// 	char	*remaining_cmd;

// 	name_start = 0;
// 	move_one_forward(redirect_str);
// 	if (redirect_str[name_start] == '>')
// 		move_one_forward(&redirect_str[name_start]);
// 	while (redirect_str[name_start] == ' ' || redirect_str[name_start] == '\t')
// 		name_start++;
// 	name_end = get_name_end(&redirect_str[name_start]);
// 	name = ft_substr(&redirect_str[name_start], 0, name_end);
// 	remaining_cmd = &redirect_str[name_start + name_end];
// 	ft_memmove(redirect_str, remaining_cmd, ft_strlen(remaining_cmd) + 2);
// 	return (name);
// }

int	redirect_input(char *input_redirect)
{
	// char	*input_redirect;
	char	*filename;
	int		fd;

	// input_redirect = get_redirect_position(command, '<');
	// if (!input_redirect)
	// 	return (SUCCESS);
	//filename = get_label_name(input_redirect);
	fd = open(input_redirect, O_RDONLY, FD_CLOEXEC);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", input_redirect, strerror(errno));
		// free(input_redirect);
		return (FAILED);
	}
	else
		redirect_fd(fd, STDIN_FILENO);
	// free(input_redirect);
	return (SUCCESS);
}


//-----------------------output-------------------------------|

int	redirect_output(char *output_redirect)
{
// 	char	*output_redirect;
// 	char	*filename;
	int		fd;
	int		open_flags;

	// output_redirect = get_redirect_position(command, '>');
	// if (!output_redirect)
	// 	return (1);

	if (output_redirect[1] == '>')
		fd = open(output_redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);


	// if (output_redirect[1] == '>')
	// 	open_flags = O_WRONLY | O_CREAT | O_APPEND;
	// else
	// 	open_flags = O_WRONLY | O_CREAT | O_TRUNC;
	// filename = get_label_name(output_redirect);
	// fd = open(filename, open_flags, 0644);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", output_redirect, strerror(errno));
		// free(filename);
		return (0);
	}
	else
		redirect_fd(fd, STDOUT_FILENO);
	// free(filename);
	return (1);
}
