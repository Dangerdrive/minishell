#include "../includes/minishell.h"

int	g_signal = 0;

int	is_fork(int status)
{
	static int	on_fork;

	if (status == 1)
		on_fork = status;
	return (on_fork);
}

int	is_after_prompt(int is_after)
{
	static int	after;

	if (is_after != -1)
		after = is_after;
	return (after);
}

int	on_heredoc(int on_heredoc)
{
	static int	heredoc;

	if (on_heredoc != -1)
		heredoc = on_heredoc;
	return (heredoc);
}

void	sigquit_case(void)
{
	// if (update_status_error(-1) == 131)
	// {
	// 	ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	// 	term_properties(1);
	// }
	// else
	// 	term_properties(0);
}

void	handle_sigint(int signal)
{
	g_signal = signal;
	if (is_fork(-1))
	{
		//free_split(envs_to_array());
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
	//ast_holder(NULL, 1, 1);
	if (on_heredoc(-1))
		close(STDIN_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	ft_putstr_fd("\n", 1);
	if (!is_after_prompt(-1))
		rl_redisplay();
	//update_status_error(130);
}

void	handle_signals(t_global **data)
{
	g_signal = 0;
	// SPECIAL BEHAVIOR FOR INTERACTIVE MODE:
	if (signal(SIGINT, handle_sigint) == SIG_ERR || signal(SIGQUIT,
			SIG_IGN) == SIG_ERR || signal(SIGTSTP, SIG_IGN) == SIG_ERR)
	{
		ft_putendl_fd("fail to set signals", 2);
		exit(EXIT_FAILURE);
	}
	(*data)->prev_process_status = g_signal;
	//IN EXECUTION MODE THE SIGNALS MUST BEHAVE LIKE IN BASH (to be implemented).
}
