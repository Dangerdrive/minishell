#include "../../includes/minishell.h"

int	pipecount(t_global *data)
{
	int	result;

	result = 0;
	while (data->hashtable[result])
	{
		result++;
	}
	return (result - 1);
}

static void	quit_child(char *(*args)[TABLE_SIZE])
{
	rl_clear_history();
	free_redir_args(args);
	close_all_fds();
	exit(EXIT_FAILURE);
}

void	handle_redirects_for_pipes(t_global *data, char *(*redirects)[TABLE_SIZE])
{
	int	i;

	i = 0;
	while ((*redirects)[i])
	{
		if (ft_strncmp((*redirects)[i], "< ", 2) == 0)
		{
			if (redirect_input((*redirects)[i]) == 0)
				quit_child(redirects);
		}
		if ((*redirects)[i][0] == '>')
		{
			if (redirect_output((*redirects)[i]) == 0)
				quit_child(redirects);
		}
		if (ft_strncmp((*redirects)[i], "<<", 2) == 0)
			redirect_heredoc(data, i, &(*redirects)[i][2]);
		i++;
	}
}

void	handle_pipe(int original_fd_out, t_global *data,
	t_tkn *current_node, t_tkn **hashtable)
{
	t_bool		is_first_node;
	t_bool		has_next_node;
	t_tkn		*last_node;
	static int	pipe_fds[2];

	last_node = hashtable[pipecount(data)];
	is_first_node = (current_node == hashtable[0]);
	has_next_node = (current_node != last_node); // Nessa comparação ele tá considerando a hashtable inteira e não apenas um índice dela (uma única lista). É isso mesmo?
	if (!is_first_node)
		redirect_fd(pipe_fds[IN], STDIN_FILENO);
	if (has_next_node)
	{
		if (pipe(pipe_fds) == -1)
			ft_dprintf(2, "minishell: pipe: %s\n", strerror(errno));
		redirect_fd(pipe_fds[OUT], STDOUT_FILENO);
	}
	else
		redirect_fd(original_fd_out, STDOUT_FILENO);
}
