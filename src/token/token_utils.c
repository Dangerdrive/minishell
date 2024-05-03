/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:01:51 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:27:26 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Initializes a hashtable by setting all entries to NULL.
 *
 * @param hashtable The hashtable to initialize.
 */
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

/**
 * Extracts a token from a string based on the specified start index and length.
 *
 * @param input The input string from which to extract the token.
 * @param i The start index for extraction.
 * @param len The length of the token to extract.
 * @return A newly allocated string containing the token, or NULL on failure.
 */
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

/**
 * Adds a new token node to a linked list of tokens.
 *
 * @param tkn_node Pointer to the first token node of the list.
 * @param content The content to store in the new token node.
 * @return Pointer to the newly added token node, or NULL on failure.
 */
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

/**
 * Marks the space_after attribute of the last node based on input characters.
 *
 * @param input The full input string.
 * @param i Index of the character after the token in the input string.
 * @param node Pointer to the first node of the token list.
 */
void	check_non_spaced_var(char *input, int i, t_tkn **node)
{
	t_tkn	*temp;

	if ((*node) && input[i - 1] != ' ')
	{
		temp = *node;
		while ((*node)->next != NULL)
			*node = (*node)->next;
		(*node)->space_after = FALSE;
		*node = temp;
	}
}

/**
 * Populates the hashtable with tokens extracted from user input.
 *
 * @param data Pointer to the global data structure.
 * @param idx Start index of the token in the user input.
 * @param len Length of the token.
 */
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
		check_non_spaced_var((*data)->usr_input, idx, &(*data)->hashtable[i]);
		while ((*data)->hashtable[i + 1] != NULL)
			i++;
	}
	add_node(&(*data)->hashtable[i], &token);
	free(token);
}
