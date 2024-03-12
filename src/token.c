/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 15:54:13 by root              #+#    #+#             */
/*   Updated: 2024/03/12 17:43:04 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_hashtable(t_tkn *(*hashtable)[TABLE_SIZE])
{
	int	i;

	i = 0;
	while (i < TABLE_SIZE)
	{
		(*hashtable)[i] = NULL;
		i++;
	}
	return ;
}

void	add_node(t_tkn **tkn_node, char *token)
{
	t_tkn	*new_node;
	t_tkn	*temp;

	temp = *tkn_node;
	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return ;
	new_node->content = token;
	new_node->next = NULL;
	if (!(*tkn_node))
		*tkn_node = new_node;
	else
	{
		while ((*tkn_node)->next != NULL)
			(*tkn_node) = (*tkn_node)->next;
		(*tkn_node)->next = new_node;
	}
	printf("%s%s%s\n", YELLOW, (*tkn_node)->content, END);
	*tkn_node = temp;
}

void	populate_hashtable(t_tkn *(*hashtable)[TABLE_SIZE], char *token)
{
	add_node(&(*hashtable)[0], token);
}

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
		init_hashtable(&(*data)->hashtable);
		populate_hashtable(&(*data)->hashtable, token);
		printf("datahash: %s\n", (*data)->((*hashtable)[0])->content);
		free(token);
		i += len;
	}
}
