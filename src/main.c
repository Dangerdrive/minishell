/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:57:43 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/08 20:42:27 by fde-alen         ###   ########.fr       */
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
	//printf("\n\n%s\n", data->cur_cwd_path);
	return (data);
}

void	handle_signal(void)
{}

void	handle_input(t_global **data)
{
	int		i;
	int		j;
	char	*token;

	(*data)->usr_input = readline((*data)->usr_input);
	if (ft_strncmp((*data)->usr_input, "exit", 5) == 0)
	{
		free((*data)->usr_input);
		return ;
	}
	i = 0;
	while ((*data)->usr_input[i] != '\0')
	{
		j = 0;
		while ((*data)->usr_input[i] == ' ')
			i++;
		while ((*data)->usr_input[j] != ' ')
			j++;
		token = ft_calloc(j + 1, sizeof(char));
		token[j] = '\0';
		while (i <= j)
		{
			token[j - i] = (*data)->usr_input[i];
			i++;
		}
		printf("%s\n", token);
		//(*data)->hashtable[0]->content = token; // needs to init first
		i++;
	}
}
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
		printf("%s%s%s\n", BLUE, data->usr_input, END);
		free(data->usr_input);
	}
	free_stuff(data);
	return (0);
}
