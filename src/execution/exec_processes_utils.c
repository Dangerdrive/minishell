/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_processes_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:58:53 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:58:55 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the termination signal of a child process is SIGINT (Ctrl-C).
 * 
 * @param[in] status The exit status of the child process.
 * @return 1 if the signal is SIGINT, otherwise 0.
 */
static int	is_control_c(int status)
{
	return (WTERMSIG(status) == SIGINT);
}

/**
 * Checks if the termination signal of a child process is SIGQUIT (Ctrl-\).
 * 
 * @param[in] status The exit status of the child process.
 * @return 1 if the signal is SIGQUIT, otherwise 0.
 */
static int	is_control_slash(int status)
{
	return (WTERMSIG(status) == SIGQUIT);
}

/**
 * Handles the signal interruption if a child process terminates due to a signal.
 * 
 * @param[in] status The exit status of the child process.
 * @param[in] is_last_child Flag to indicate if this is the last child.
 * @return Modified exit status incorporating the signal number.
 */
int	handle_signal_interrupt(int status, int is_last_child)
{
	if (is_control_c(status))
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	if (is_control_slash(status) && is_last_child)
	{
		ft_putstr_fd("Quit\n", STDOUT_FILENO);
	}
	return (INTERRUPT + WTERMSIG(status));
}

/**
 * Waits for a child process to terminate and returns its exit status.
 * 
 * @param[in] child_pid PID of the child process.
 * @param[in] is_last_child Flag to indicate if this is the last child process.
 * @return The exit status of the child, or EXIT_FAILURE on error.
 */
int	wait_for_child(int child_pid, int is_last_child)
{
	int	status;

	if (child_pid == FORK_ERROR)
		return (EXIT_FAILURE);
	if (waitpid(child_pid, &status, 0) == -1)
		ft_dprintf(2, "minishell: waitpid: %s\n", strerror(errno));
	if (WIFSIGNALED(status))
		return (handle_signal_interrupt(status, is_last_child));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

/**
 * Waits for all child processes specified in the array to terminate, handling
 * their respective exit statuses.
 * 
 * @param[in] children_pid Array of PIDs for the child processes.
 * @return The exit status of the last child process.
 */
int	wait_for_children(int children_pid[1024])
{
	int	i;
	int	exit_status;
	int	is_last_child;

	i = 0;
	exit_status = 0;
	while (children_pid[i] != 0)
	{
		is_last_child = children_pid[i + 1] == 0;
		exit_status = wait_for_child(children_pid[i], is_last_child);
		i++;
	}
	close_extra_fds();
	return (exit_status);
}
