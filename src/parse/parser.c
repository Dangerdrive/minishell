/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:40 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:10:08 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Determines the type of a token based on its content and position 
 * in the token list.
 *
 * @param node Pointer to the token whose type is to be determined.
 * @return The type of the token as a string constant.
 */
char	*get_tkn_type(t_tkn *node)
{
	if (!node->type)
	{
		if (node->content[0] == SIMPLE_QUOTE)
			return (STRING_STRONG);
		else if (node->content[0] == DOUBLE_QUOTE)
			return (STRING_STD);
		else if (is_special_token(node->content))
			return (SPECIAL_CHAR);
		else if ((node->content[0] == '$'
				&& identifier_is_valid(node->content + 1))
			|| !strcmp(node->content, "$?")
			|| is_special_variable(node->content, FALSE))
			return (VARIABLE);
		else if (is_export_var(node->content))
			return (EXPT_VARIABLE);
		else if (!node->prev || is_pipe(node->prev->content))
			return (COMMAND);
		else
			return (ARGUMENT);
	}
	return (node->type);
}

/**
 * Removes single or double quotes from the content of a token.
 *
 * @param content Pointer to the string to modify.
 */
void	remove_quotes(char **content)
{
	char	*new_content;
	int		len;
	int		i;

	len = ft_strlen(*content) - 1;
	if (ft_strcmp(*content, PIPE)
		&& ((*content)[0] == 34 || (*content)[0] == 39))
	{
		new_content = ft_calloc(len, sizeof(char));
		i = 1;
		while (i < len)
		{
			new_content[i - 1] = (*content)[i];
			i++;
		}
		ft_strlcpy(*content, new_content, len);
		free(new_content);
	}
}

/**
 * Handles the concatenation of export variable tokens.
 *
 * @param node Pointer to the current token node in the token list.
 */
void	handle_export(t_tkn **node)
{
	char	*new_content;
	t_tkn	*head;
	t_tkn	*temp;

	new_content = NULL;
	head = *node;
	*node = (*node)->next;
	while ((*node) && is_export_var((*node)->content) && (*node)->next)
	{
		new_content = ft_strjoin((*node)->content, (*node)->next->content);
		free((*node)->content);
		(*node)->content = ft_strdup(new_content);
		temp = (*node)->next->next;
		if (temp)
			temp->prev = *node;
		free(new_content);
		free((*node)->next->content);
		free((*node)->next);
		(*node)->next = temp;
		*node = (*node)->next;
	}
	*node = head;
}

/**
 * Parses a hashtable of tokens, determining types, removing quotes, and handling
 * exports.
 *
 * @param hashtable Pointer to the hashtable of tokens.
 * @param data Pointer to global data structure.
 * @return Syntax correctness indicator (0 for correct, 1 for error).
 */
int	parse(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data)
{
	int		i;
	int		syntax;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			(*hashtable)[i]->type = get_tkn_type((*hashtable)[i]);
			remove_quotes(&(*hashtable)[i]->content);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		if (!ft_strcmp((*hashtable)[i]->content, "export"))
			handle_export(hashtable[i]);
		i++;
	}
	syntax = 0;
	if (expand(hashtable, data) == 1)
		syntax = lexer(hashtable);
	return (syntax);
}
