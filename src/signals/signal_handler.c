#include "../includes/minishell.h"

int	g_signal;

int	check_fork(int status)
{
	static int	on_fork;

	if (status == 1)
		on_fork = status;
	return (on_fork);
}

int	check_heredoc(int on_heredoc)
{
	static int	heredoc;

	if (on_heredoc != -1)
		heredoc = on_heredoc;
	return (heredoc);
}

int	is_after_prompt(int is_after)
{
	static int	after;

	if (is_after != -1)
		after = is_after;
	return (after);
}

// void	handle_sigquit(void)
// {
// 	if (update_status_error(-1) == 131)
// 	{
// 		ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
// 		term_properties(1);
// 	}
// 	else
// 		term_properties(0);
// }

void	handle_sigint(int signal)
{
	g_signal = signal;
	if (check_fork(-1))
	{
		//free_split(envs_to_array());
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
	//ast_holder(NULL, 1, 1);
	if (check_heredoc(-1))
		close(STDIN_FILENO);
	//rl_on_new_line();
	rl_replace_line("", 0);
	//rl_redisplay();
	printf("\n");
	// if (!is_after_prompt(-1))
	// 	rl_redisplay();
	g_signal = 130;
	//printf("g_signal = %d\n", g_signal);
}

void	handle_signals(t_global **data)
{
	printf("%s%s%s ", BLUE, (*data)->cur_path, END);
	if (signal(SIGINT, handle_sigint) == SIG_ERR || signal(SIGQUIT,
			SIG_IGN) == SIG_ERR || signal(SIGTSTP, SIG_IGN) == SIG_ERR)
	{
		ft_putendl_fd("fail to set signals", 2);
		exit(EXIT_FAILURE);
	}
	printf("g_signal = %d\n", g_signal);
	(*data)->prev_process_status = g_signal;
	// if (g_signal)
	// {
	// 	printf("%s%s%s ", BLUE, (*data)->cur_path, END);
	// }
	//IN EXECUTION MODE THE SIGNALS MUST BEHAVE LIKE IN BASH (to be implemented).
}
