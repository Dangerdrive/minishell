/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:59:19 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:59:20 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles the execution of commands stored in the global data's hashtable,
 * managing single and multiple processes based on pipe presence.
 *
 * @param[in, out] data Global structure containing execution details.
 * @return Execution result stored in data->ret.
 */
int	handle_execution(t_global *data)
{
	char	**args;

	args = NULL;
	if (data->hashtable[0] && data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (pipecount(data) == 0)
		data->ret = exec_one_process(data);
	else if (pipecount(data) > 0)
		data->ret = exec_processes(data);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	return (data->ret);
}
