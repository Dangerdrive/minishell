/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:57:43 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/15 16:24:34 by gde-souz         ###   ########.fr       */
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
		printf("%s ", data->cur_path);
		handle_signal();
		if (handle_input(&data) == -1)
			break ;
		ft_memdel(data->usr_input);
		// if (data->exit == true)
		// 	break ;
	}
	clean_stuff(&data);
	return (0);
}
