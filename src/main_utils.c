/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:49:46 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/13 19:40:59 by fde-alen         ###   ########.fr       */
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
	data->exit = false;
	init_hashtable(&data->hashtable);
	//printf("\n\n%s\n", data->cur_cwd_path);
	return (data);
}

void	handle_signal(void)
{}

void	free_hashtable(t_global **data)
{
	int		i;
	t_tkn	*temp;

	i = 0;
	//while (i < TABLE_SIZE)
	while ((*data)->hashtable[i])
	{
		while ((*data)->hashtable[i])
		{
			temp = (*data)->hashtable[i]->next;
			free((*data)->hashtable[i]->content);
			free((*data)->hashtable[i]);
			(*data)->hashtable[i] = temp;
		}
		//ft_memdel((*data)->hashtable[i]);
		i++;
	}
	//ft_memdel((*data)->hashtable);
	//ft_memdel((*data)->hashtable[i]);
}

// void	free_stuff(t_global *data)
// {
// 	free(data->cur_cwd_path);
// 	free_env(&data);
// 	//ft_memdel(data->usr_input);
// 	free_hashtable(&data);
// 	//free(data);
// 	rl_clear_history();
// 	free(&data);
// }

void free_stuff(t_global **data)
{
	if (data == NULL || *data == NULL)
		return;
	if ((*data)->cur_cwd_path != NULL)
	{
		free((*data)->cur_cwd_path);
		(*data)->cur_cwd_path = NULL;
	}
	free_env(data);
	free_hashtable(data);
	rl_clear_history();
	ft_memdel((*data)->usr_input);
	ft_memdel(*data);
	*data = NULL;
}


