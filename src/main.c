/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:57:43 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/11 18:44:06 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	*g_signal;

t_global	*init_data(void)
{
	t_global	*data;

	data = ft_calloc(1, sizeof(t_global));
	if (!data)
		return (NULL);
	data->env = __environ;
	data->cur_cwd_path = getcwd(NULL, 0);
	//data->hashtable = NULL;
	//printf("\n\n%s\n", data->cur_cwd_path);
	return (data);
}

void	handle_signal(void)
{}

void	free_stuff(t_global *data)
{
	free(data->cur_cwd_path);
	free_env(&data);
	free(data);
}

int	main(void)
{
	static t_global	*data;

	data = init_data();
	if (copy_env(&data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->usr_input = NULL;
	while (1)
	{
		handle_signal();
		handle_input(&data);
		//printf("%s%s%s\n", BLUE, data->usr_input, END);
		free(data->usr_input);
	}
	free_stuff(data);
	return (0);
}
