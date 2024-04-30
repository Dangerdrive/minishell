/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:20:55 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:20:56 by gde-souz         ###   ########.fr       */
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

char	*get_token(char *input, int i, int len)
{
	char	*token;

	token = ft_calloc(len + 1, sizeof(char));
	if (!token)
		return (NULL);
	while (--len >= 0)
		token[len] = input[i + len];
	return (token);
}

t_tkn	*add_node(t_tkn **tkn_node, char **content)
{
	t_tkn	*new_node;
	t_tkn	*temp;

	new_node = ft_calloc(1, sizeof(t_tkn));
	temp = NULL;
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup(*content);
	new_node->space_after = TRUE;
	new_node->next = NULL;
	if (!(*tkn_node))
		(*tkn_node) = new_node;
	else
	{
		temp = (*tkn_node);
		while ((*tkn_node)->next != NULL)
			(*tkn_node) = (*tkn_node)->next;
		(*tkn_node)->next = new_node;
		new_node->prev = (*tkn_node);
		*tkn_node = temp;
	}
	return (new_node);
}

void	check_non_spaced_var(char *input, int i, t_tkn **node)
{
	t_tkn	*temp;

	if ((*node) && !strcmp((*node)->content, "echo")
		&& input[i - 1] != ' ')
	{
		temp = *node;
		while ((*node)->next != NULL)
			*node = (*node)->next;
		(*node)->space_after = FALSE;
		*node = temp;
	}
}

void	populate_hashtable(t_global **data, int idx, int len)
{
	char	*token;
	int		i;

	token = get_token((*data)->usr_input, idx, len);
	i = 0;
	if (ft_strncmp(token, PIPE, 1) == 0)
	{
		while ((*data)->hashtable[i])
			i++;
	}
	else
	{
		if ((*data)->hashtable[i]
			&& strcmp((*data)->hashtable[i]->content, "echo") == 0)
			check_non_spaced_var((*data)->usr_input, idx,
				&(*data)->hashtable[i]);
		while ((*data)->hashtable[i + 1] != NULL)
			i++;
	}
	add_node(&(*data)->hashtable[i], &token);
	free(token);
}
