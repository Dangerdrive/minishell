/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:01:20 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:24:11 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Handles SIGINT (Ctrl-C) signal during prompt by printing a newline and
 * reinitializing the readline interface.
 *
 * @param sig The signal number (not used in this function).
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * Sets signal handling for the interactive shell prompt, ignoring SIGQUIT
 * and handling SIGINT to allow graceful continuation of the command prompt.
 *
 * @param data Unused in this function, reserved for future use.
 */
void	define_prompt_signals(t_global **data)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_handler = &handle_sigint;
	sa_sigint.sa_flags = 0;
	(void)data;
	sigemptyset(&sa_sigint.sa_mask);
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

/**
 * Defines signal handling for commands during execution, setting default
 * behaviors or ignoring signals based on the process ID.
 *
 * @param pid Process ID (0 for child process, non-zero for parent).
 */
void	define_exec_signals(int pid)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);
	if (pid == 0)
		sa_sigint.sa_handler = SIG_DFL;
	else
		sa_sigint.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	if (pid == 0)
		sa_sigquit.sa_handler = SIG_DFL;
	else
		sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

/**
 * Sets signal handling specifically for heredoc processes, ignoring
 * interrupts in parent process and default actions in child.
 *
 * @param pid Process ID (0 for child process, non-zero for parent).
 */
void	define_heredoc_signals(int pid)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);
	if (pid == 0)
		sa_sigint.sa_handler = SIG_DFL;
	else
		sa_sigint.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}
