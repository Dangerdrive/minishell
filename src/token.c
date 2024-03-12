/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:54:13 by root              #+#    #+#             */
/*   Updated: 2024/03/12 16:46:32 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_input(t_global **data)
{
	int		i;
	int		j;
	int		len;
	char	*token;

	(*data)->usr_input = readline((*data)->usr_input);
	if (ft_strncmp((*data)->usr_input, "exit", 5) == 0)
	{
		free((*data)->usr_input);
		return ;
	}
	i = 0;
	while ((*data)->usr_input[i])
	{
		j = 0;
		while ((*data)->usr_input[i] == ' ')
			i++;
		while ((*data)->usr_input[i + j] != ' '
			&& (*data)->usr_input[i + j] != '\0')
			j++;
		len = j;
		token = ft_calloc(len + 1, sizeof(char));
		token[j] = '\0';
		while (--j >= 0)
			token[j] = (*data)->usr_input[i + j];
		printf("%s%s%s\n", BLUE, token, END);
		//populate_hashtable(&(*data)->hashtable, token);
		free(token);
		i += len;
	}
}
