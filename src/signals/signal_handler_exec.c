#include "../includes/minishell.h"

int	g_signal = 0;

void interrupt_handler(int signal)
{
	(void)signal;
    printf("SIGINT received, stopping program...\n");
	g_signal = 1;
}

// Function to handle the EOF signal (Ctrl-D)
void handle_eof(int sig)
{
	(void)sig;
    printf("\nCaught EOF signal. Exiting...\n");
    g_signal = 1;
}

void handle_backslash(int sig)
{
	(void)sig;
    printf("\nCaught backslash signal.\n"); //TO CONTINUE...
    g_signal = 1;
}

int	handle_ctrl_c(t_global **data)
{
	struct sigaction sa;

    sa.sa_handler = interrupt_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

	 if (sigaction(SIGINT, &sa, NULL) == -1)
	{
        perror("sigaction");
        return (EXIT_FAILURE);
    }
	if (g_signal == 1)
	{
		(*data)->exit = 1;
		(*data)->prev_process_status = 130;
		g_signal = 0;
		return (1);
	}
    return (0);
}

int	handle_ctrl_d(t_global **data)
{
	struct sigaction sa;

    sa.sa_handler = handle_eof;
    sigaction(SIGQUIT, &sa, NULL);

	 if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
        perror("sigaction");
        return (EXIT_FAILURE);
    }
	if (g_signal == 1)
	{
		(*data)->exit = 1;
		(*data)->prev_process_status = 0;
		g_signal = 0;
		return (1);
	}
    return (0);
}

int	handle_ctrl_slash(t_global **data)
{
	struct sigaction sa;

    sa.sa_handler = handle_backslash;
    sigaction(SIGABRT, &sa, NULL);
    return (0);
}

void	handle_signals_exec(t_global **data)
{
	//IN EXECUTION MODE THE SIGNALS MUST BEHAVE LIKE IN BASH (to be implemented).
    handle_ctrl_c(data);
	handle_ctrl_d(data);
	handle_ctrl_slash(data);

}