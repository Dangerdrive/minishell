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

void	handle_sigquit(int signal)
{
	if (signal == 131)
	{
		g_signal = signal;
		ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		//term_properties(1);
	}
	else
		//term_properties(0);
	exit(EXIT_FAILURE);
}

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
	{
		if (signal(SIGQUIT, handle_sigquit) == SIG_ERR
			|| signal(SIGINT, handle_sigint) == SIG_ERR)
		{
			ft_putendl_fd("fail to set signals", 2);
			exit(EXIT_FAILURE);
		}
	}
	(*data)->prev_process_status = g_signal;
	//IN EXECUTION MODE THE SIGNALS MUST BEHAVE LIKE IN BASH (to be implemented).
}

// FUNCIONAMENTO PADRÃO:::

// CTRL-C = INTERUPTS EXECUTION
// CTRL-D = EOF (end-of-file) SIGNAL
// CTRL-\ = SIGQUIT - FORCED EXIT
