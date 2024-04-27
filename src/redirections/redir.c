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

void	write_in_heredoc(t_global *data, int heredoc_number, char *eof)
{
	int		tmp_file_fd;
	char	*filename;
	char	*line;

	tmp_file_fd = 0;
	filename = tmp_filename(heredoc_number);
	tmp_file_fd = get_fd(filename);
	line = readline("> ");
	while (line && ft_strncmp(line, eof, (ft_strlen(eof) + 1)))
	{
		expand_heredoc(data, &line); // ADD EXPANSÃO DE $?
		ft_putstr_fd(line, tmp_file_fd);
		ft_putchar_fd('\n', tmp_file_fd);
		free(line);
		line = readline("> ");
	}
	if (!line)
		ft_dprintf(STDOUT_FILENO, "%sminishell: warning: heredoc delimited by EOF (wanted '%s')%s\n", YELLOW, eof, END);
	//add_history();
	close(tmp_file_fd);
	free(filename);
	free(line);
	redirect_fd(tmp_file_fd, STDIN_FILENO);
	exit(EXIT_SUCCESS);
}

void	redirect_heredoc(t_global *data, int heredoc_number, char *eof)
{
	int		pid;

	pid = fork();
	define_heredoc_signals(pid);
	if (pid == -1)
	{
		perror("fork");
        exit(EXIT_FAILURE);
	}
	if (pid == 0)
		write_in_heredoc(data, heredoc_number, eof);
	else
	{
		// perror("waitpid");
		// exit(EXIT_FAILURE);
		data->ret = wait_for_child(pid, TRUE);
		define_prompt_signals();
	}
	//salvar histórico heredoc
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
