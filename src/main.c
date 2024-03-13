/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:57:43 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/12 21:03:51 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	static t_global	*data;
	t_tkn			*temp;

	data = init_data();
	if (copy_env(&data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->usr_input = NULL;
	while (1)
	{
		handle_signal();
		handle_input(&data);
		temp = data->hashtable[0];
		while (temp != NULL)
			temp = temp->next;
		ft_memdel(data->usr_input);
		if (data->exit == true)
			break ;
	}
	free_stuff(data);
	return (0);
}
