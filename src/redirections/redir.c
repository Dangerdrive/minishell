#include "../../includes/minishell.h"

void	write_in_heredoc(t_global *data, int heredoc_number, char *eof)
{
	int		tmp_file_fd;
	char	*filename;
	char	*line;

	tmp_file_fd = 0;
	filename = tmp_filename(heredoc_number);
	tmp_file_fd = get_fd(filename);
	// printf("open_redirs %d\n", data->open_redirs);
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

// void	redirect_heredoc(t_global *data, int heredoc_number, char *eof)
// {
// 	int		pid;
// 	int		open_redirs;

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	data->sig_env = HEREDOC;
// 	define_heredoc_signals(pid);
// 	if (pid == 0)
// 		write_in_heredoc(data, heredoc_number, eof, &(1));
// 	else
// 	{
// 		data->ret = wait_for_child(pid, TRUE);
// 	}
// 	data->sig_env = PROMPT;
// 	define_prompt_signals(&data);
// 	external_exit(EXIT_SUCCESS);
// }

// int	redirect_input(char *input_redirect)
// {
// 	int		fd;

// 	fd = open(&input_redirect[2], O_RDONLY, FD_CLOEXEC);
// 	if (fd == -1)
// 	{
// 		ft_dprintf(STDERR_FILENO, "open: %s: %s\n",
// 			&input_redirect[2], strerror(errno));
// 		return (0);
// 	}
// 	else
// 		redirect_fd(fd, STDIN_FILENO);
// 	return (1);
// }

// int	redirect_output(char *output_redirect)
// {
// 	int		fd;

// 	if (output_redirect[1] == '>')
// 		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		ft_dprintf(STDERR_FILENO, "open: %s: %s\n",
// 			&output_redirect[2], strerror(errno));
// 		return (0);
// 	}
// 	else
// 		redirect_fd(fd, STDOUT_FILENO);
// 	return (1);
// }


int	redirect_input(char *input_redirect, int i)
{
	int		fd;
	char 	*filename;

	if (input_redirect[1] == '<')
	{
		filename = tmp_filename(i);
		fd = open(filename, O_RDONLY, FD_CLOEXEC);
		free(filename);
	}
	else
		fd = open(&input_redirect[2], O_RDONLY, FD_CLOEXEC);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n",
			&input_redirect[2], strerror(errno));
		return (0);
	}
	else
		redirect_fd(fd, STDIN_FILENO);
	return (1);
}

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
