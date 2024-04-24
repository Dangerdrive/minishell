#include "../../includes/minishell.h"

static char	*tmp_filename(int heredoc_number)
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

void	write_in_heredoc(t_global *data, int tmp_file_fd, char *eof)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!ft_strncmp(line, eof, (ft_strlen(eof) + 1)))
		{
			close(tmp_file_fd);
			free(line);
			break ;
		}
		else
		{
			expand_heredoc(data, &line);
			ft_putstr_fd(line, tmp_file_fd);
			ft_putchar_fd('\n', tmp_file_fd);
		}
		free(line);
	}
	return ;
}

void	redirect_heredoc(t_global *data, int pid, int heredoc_number, char *eof)
{
	char	*filename;
	int		tmp_file_fd;

	filename = tmp_filename(heredoc_number);
	tmp_file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_file_fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", filename, strerror(errno));
		free(filename);
		return ;
	}
	(void)pid;
	//define_heredoc_signals(pid);
	write_in_heredoc(data, tmp_file_fd, eof);
	//salvar histórico heredoc
	close(tmp_file_fd);
	free(filename);
	redirect_fd(tmp_file_fd, STDIN_FILENO);
}

int	redirect_input(char *input_redirect)
{
	int		fd;

	fd = open(&input_redirect[2], O_RDONLY, FD_CLOEXEC);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", &input_redirect[2], strerror(errno));
		// free(input_redirect);
		return (0);
	}
	else
		redirect_fd(fd, STDIN_FILENO);
	// free(input_redirect);
	return (1);
}

int	redirect_output(char *output_redirect)
{
	int		fd;

	if (output_redirect[1] == '>')
		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", &output_redirect[2], strerror(errno));
		// free(filename);
		return (0);
	}
	else
		redirect_fd(fd, STDOUT_FILENO);
	// free(filename);
	return (1);
}
