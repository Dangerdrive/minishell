/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:49:46 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/15 17:29:53 by gde-souz         ###   ########.fr       */
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
	data->usr_input = NULL;
	data->env = __environ;
	data->cur_path = getcwd(NULL, 0);
	data->exit = false;
	init_hashtable(&data->hashtable);
	return (data);
}

void	handle_signal(void)
{}

void	free_hashtable(t_global **data)
{
	int		i;
	t_tkn	*temp;

	i = 0;
	temp = NULL;
	while ((*data)->hashtable[i] != NULL)
	{
		temp = (*data)->hashtable[i]->next;
		free((*data)->hashtable[i]->content);
		free((*data)->hashtable[i]);
		(*data)->hashtable[i] = temp;
	}
}

// void	free_stuff(t_global *data)
// {
// 	free(data->cur_path);
// 	free_env(&data);
// 	//ft_memdel(data->usr_input);
// 	free_hashtable(&data);
// 	//free(data);
// 	rl_clear_history();
// 	free(&data);
// }

void	clean_stuff(t_global **data)
{
	if (data == NULL || *data == NULL)
		return ;
	if ((*data)->cur_path != NULL)
	{
		free((*data)->cur_path);
		(*data)->cur_path = NULL;
	}
	free_env(data);
	free_hashtable(data);
	rl_clear_history();
	ft_memdel((*data)->usr_input);
	ft_memdel(*data);
	*data = NULL;
}
