#include "../includes/minishell.h"

/**
 * Determines if a character is a special variable character.
 *
 * @param c The character to check.
 * @return True if it is a special variable character, otherwise false.
 */
t_bool	is_special_var_char(char c)
{
	if ((c >= 32 && c <= 47 && c != 35) || (c >= 58 && c <= 64 && c != 63)
		|| (c >= 93 && c <= 96 && c != '_') || (c >= 123 && c <= 126))
	{
		return (true);
	}
	return (false);
}

/**
 * Checks if a variable is a special shell variable.
 *
 * @param var The variable to check.
 * @param print Whether to print a message if the variable is special.
 * @return True if the variable is special, otherwise false.
 */
t_bool	is_special_variable(char *var, t_bool print)
{
	if (!ft_strcmp(var, "$#") || !ft_strcmp(var, "$!") || !ft_strcmp(var, "$@")
		|| !ft_strcmp(var, "$$") || !ft_strcmp(var, "$0")
		|| !ft_strcmp(var, "$-") || !ft_strcmp(var, "$*"))
	{
		if (print)
			ft_dprintf(2, "%s\nThis functionality is beyond Minishell's \
			scope, ****@#$@***.\n\n%s", RED, END);
		return (true);
	}
	return (false);
}

/**
 * Determines if the content at a specific position in a token node is a 
 * variable.
 *
 * @param node The token node.
 * @param i The index in the content.
 * @param len The length after index to check.
 * @return True if it's a variable name, otherwise false.
 */
t_bool	is_var_name(t_tkn **node, int i, int len)
{
	if (((*node)->type[0] == 'v' && (*node)->content[i + len]
			&& !is_special_var_char((*node)->content[i + len]))
		|| ((*node)->type[0] == 's' && (*node)->content[i + len]
			&& !is_special_var_char((*node)->content[i + len])))
		return (TRUE);
	if (!ft_strcmp((*node)->type, EXPT_VARIABLE) && (*node)->content[i + len]
		&& !is_special_var_char((*node)->content[i + len]))
		return (TRUE);
	return (FALSE);
}

/**
 * Adds a new token node before the specified node.
 *
 * @param node The current node to insert before.
 * @param i The index in the content up to which the new node will include.
 */
void	add_node_before(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return ;
	new_node->content = ft_strndup((*node)->content, i - 1);
	new_node->type = STRING_STD;
	new_node->next = (*node);
	new_node->prev = (*node)->prev;
	(*node)->prev->next = new_node;
}

/**
 * Adds a new token node after the specified node.
 *
 * @param node The current node to insert after.
 * @param i The index in the content from which the new node will start.
 * @return The new node or NULL if allocation fails.
 */
t_tkn	*add_node_after(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup((*node)->content + i);
	new_node->type = STRING_STD;
	new_node->next = (*node)->next;
	new_node->prev = *node;
	if ((*node)->next)
		(*node)->next->prev = new_node;
	return (new_node);
}
