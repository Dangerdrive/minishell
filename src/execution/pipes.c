#include "../../includes/minishell.h"

/**
 * Counts the number of command nodes in the global hashtable.
 *
 * @param[in] data Pointer to the global data structure.
 * @return Number of pipes needed based on the count of command nodes.
 */
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

/**
 * Cleans up resources and exits the process when a child process needs to quit.
 *
 * @param[in] args Pointer to the array of command arguments.
 */
static void	quit_child(char *(*args)[TABLE_SIZE])
{
	rl_clear_history();
	free_redir_args(args);
	close_all_fds();
	exit(EXIT_FAILURE);
}

/**
 * Handles input and output redirections for commands involved in piping.
 *
 * @param[in, out] redirects Array of redirection commands.
 */
void	handle_redirects_for_pipes(char *(*redirects)[TABLE_SIZE])
{
	int	i;

	i = 0;
	while ((*redirects)[i])
	{
		if ((*redirects)[i][0] == '<')
		{
			if (redirect_input((*redirects)[i], i) == 0)
				quit_child(redirects);
		}
		if ((*redirects)[i][0] == '>')
		{
			if (redirect_output((*redirects)[i]) == 0)
				quit_child(redirects);
		}
		i++;
	}
}

/**
 * Manages file descriptors for processes in a pipeline.
 *
 * @param[in] original_fd_out Original standard output file descriptor.
 * @param[in, out] data Pointer to the global data structure.
 * @param[in] current_node Current command node being processed.
 * @param[in] hashtable Array of all command nodes.
 */
void	handle_pipe(int original_fd_out, t_global *data,
	t_tkn *current_node, t_tkn **hashtable)
{
	t_bool		is_first_node;
	t_bool		has_next_node;
	t_tkn		*last_node;
	static int	pipe_fds[2];

	last_node = hashtable[pipecount(data)];
	is_first_node = (current_node == hashtable[0]);
	has_next_node = (current_node != last_node);
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
