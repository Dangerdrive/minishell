/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:57:43 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/13 16:20:23 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (handle_input(&data) == 0)
		{
			free_stuff(data);
			break ;
		}
		ft_memdel(data->usr_input);
		// if (data->exit == true)
		// 	break ;
	}
	free_stuff(data);
	return (0);
}
