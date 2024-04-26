#include "../../includes/minishell.h"

void	save_original_fd_in(int original_fds[2])
{
	if (original_fds[IN] == -1)
		original_fds[IN] = dup(STDIN_FILENO);
}

void	save_original_fd_out(int original_fds[2])
{
	if (original_fds[OUT] == -1)
		original_fds[OUT] = dup(STDOUT_FILENO);
}

int	handle_input_redirect(char *command, int original_fds[2])
{
	save_original_fd_in(original_fds);
	if (redirect_input(command) == 0)
	{
		redirect_fd(original_fds[IN], STDIN_FILENO);
		return (0);
	}
	return (1);
}

int	handle_output_redirect(char *command, int original_fds[2])
{
	save_original_fd_out(original_fds);
	if (redirect_output(command) == 0)
	{
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
		return (0);
	}
	return (1);
}


