/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:21:48 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:21:49 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	data->is_exec = FALSE;
	data->is_heredoc = FALSE;
	data->exported = NULL;
	data->usr = ft_strdup(ft_getenv("USER", &data));
	init_hashtable(&data->hashtable);
	return (data);
}

int	main(void)
{
	static t_global	*data;
	int				result;

	data = init_data();
	data->usr_input = NULL;
	while (!data->exit)
	{
		define_prompt_signals();
		if (!data->exit && handle_prompt(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	result = data->ret;
	clean_stuff(&data);
	return (result);
}
