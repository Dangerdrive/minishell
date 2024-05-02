#include "../../includes/minishell.h"

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

// void	heredoc_prompt(char **line, int heredoc_number, t_global *data)
// {
// 	(void)data;
// 	(void)heredoc_number;
// 	// printf("%d", heredoc_number);
// 	// if (heredoc_number == 0)
// 		*line = readline("> ");
// 	// else
// 	// 	*line = readline("");
// }

void	handle_heredoc_ctrl_d(t_global *data, char *eof)
{
	ft_dprintf(STDOUT_FILENO,
		"%sminishell: warning: heredoc delimited by EOF (wanted '%s')%s\n",
	YELLOW, eof, END);
	data->ret = 0;
}
