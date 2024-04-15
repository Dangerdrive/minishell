#include "../includes/minishell.h"

int	g_sig;

void	handle_sigquit(int signal)
{
	if (signal == 131)
	{
		g_sig = signal;
		ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		//term_properties(1);
	}
	else
		//term_properties(0);
	exit(EXIT_FAILURE);
}

void	handle_sigint_exec(int signal)
{
	g_sig = signal;
	exit(EXIT_SUCCESS);
	rl_replace_line("", 0);
	printf("\n");
	g_sig = 130;
}

void	handle_signals_exec(t_global **data)
{
		if (signal(SIGQUIT, handle_sigquit) == SIG_ERR
			|| signal(SIGINT, handle_sigint_exec) == SIG_ERR)
		{
			ft_putendl_fd("fail to set signals", 2);
			exit(EXIT_FAILURE);
		}
	(*data)->prev_process_status = g_sig;
}

// FUNCIONAMENTO PADRÃO:::

// CTRL-C = INTERUPTS EXECUTION
// CTRL-D = EOF (end-of-file) SIGNAL
// CTRL-\ = SIGQUIT - FORCED EXIT
