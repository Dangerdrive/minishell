/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:54:13 by root              #+#    #+#             */
/*   Updated: 2024/03/11 18:56:47 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	populate_hashtable(t_hash **hash_node, char *token)
// {
// 	t_hash	*new_node;

// 	new_node = ft_calloc(1, sizeof(t_hash));
// 	if (!new_node)
// 		return ;
// 	new_node->content = token;
// 	new_node->next = NULL;
// 	if (!(*hash_node))
// 		*hash_node = new_node;
// 	else
// 		(*hash_node)->next = new_node;
// }

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
		//populate_hashtable(&(*data)->hashtable[0], token);
		free(token);
		i += len;
	}
}
