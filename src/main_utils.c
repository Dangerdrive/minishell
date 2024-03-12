/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:49:46 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/12 18:50:36 by fde-alen         ###   ########.fr       */
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

void	free_hashtable(t_global **data)
{
	int	i;

	i = 0;
	while (i < TABLE_SIZE)
	{
		ft_memdel((*data)->hashtable[i]);
		i++;
	}
}

void	free_stuff(t_global *data)
{
	free(data->cur_cwd_path);
	free_env(&data);
	free_hashtable(&data);
	free(data);
}
