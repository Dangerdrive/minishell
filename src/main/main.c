/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:20 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:04:18 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Initializes and allocates memory for the "global" data structure, setting up
 * the initial state of the shell environment.
 *
 * @return Pointer to the newly created global data structure or NULL if
 *         memory allocation fails.
 */
t_global	*init_data(void)
{
	t_global	*data;

	data = ft_calloc(1, sizeof(t_global));
	if (!data)
		return (NULL);
	data->usr_input = NULL;
	data->env = NULL;
	data->cur_path = getcwd(NULL, 0);
	data->ret = 0;
	data->exit = 0;
	data->original_stdin = STDIN_FILENO;
	data->original_stdout = STDOUT_FILENO;
	data->env = ft_strarr_dup(__environ);
	data->exported = NULL;
	data->open_redirs = 0;
	data->usr = ft_strdup(ft_getenv("USER", &data));
	init_hashtable(&data->hashtable);
	return (data);
}

/**
 * The main function of the shell. Manages shell initialization, command
 * processing loop, and cleanup.
 *
 * @return The exit status of the shell.
 */
int	main(void)
{
	static t_global	*data;
	int				result;

	data = init_data();
	data->usr_input = NULL;
	while (!data->exit)
	{
		data->sig_env = PROMPT;
		define_prompt_signals(&data);
		if (!data->exit && handle_prompt(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	result = data->ret;
	clean_stuff(&data);
	return (result);
}
