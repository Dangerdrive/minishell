#include "../includes/minishell.h"

int	g_signal;

// void	term_properties(int restore)
// {
// 	static struct termios	term;

// 	if (!restore)
// 		tcgetattr(STDIN_FILENO, &term);
// 	else
// 		tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }

void	handle_sigint(int signal)
{
	g_signal = signal;
	rl_replace_line("", 0);
	printf("\n");
	g_signal = 130;
}

void	handle_signals(t_global **data)
{
	// printf("%s%s%s ", BLUE, (*data)->cur_path, END);
	if (!(*data)->is_exec)
	{
		if (signal(SIGQUIT, SIG_IGN) == SIG_ERR
			|| signal(SIGINT, handle_sigint) == SIG_ERR)
		{
			ft_putendl_fd("fail to set signals", 2);
			exit(EXIT_FAILURE);
		}
	}
	else
		handle_signals_exec(data);
	(*data)->prev_process_status = g_signal;
}
