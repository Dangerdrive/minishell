#include "../includes/minishell.h"

/**
 * Checks if the input at a specific index starts with a valid command.
 *
 * @param[in] node Pointer to the token node being checked.
 * @param[in] i Index in the token array.
 * @return 1 if valid command found, 0 otherwise, with error message output.
 */
int	check_valid_input(t_tkn **node)
{
	if ((is_double_special_token((*node)) && !(*node)->delimiter)
		|| (!(*node)->next && !ft_strcmp((*node)->type, SPECIAL_CHAR)
			&& !(*node)->delimiter) || is_and_or((*node)->content))
	{
		ft_dprintf(2, "Syntax error.\n");
		return (0);
	}
	return (1);
}

/**
 * Traverses through token nodes and processes redirections.
 *
 * @param[in, out] node Pointer to the current token node to check.
 */
void	check_redirects(t_tkn **node)
{
	t_tkn	*head;

	head = *node;
	while (*node)
	{
		while ((*node)->content && is_redir((*node)->content)
			&& (*node)->next)
		{
			parse_redir(node, &head);
		}
		*node = (*node)->next;
	}
	*node = head;
}

/**
 * Performs lexical analysis on a hashtable of tokens, checking commands
 * and processing redirections.
 *
 * @param[in, out] hashtable Pointer to the hashtable of token nodes.
 * @return 1 on successful analysis, 0 on finding an error.
 */
int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int		i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!check_valid_input(&(*hashtable)[i]))
			{
				(*hashtable)[i] = temp;
				return (0);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		remove_pipe(&(*hashtable)[i], i);
		check_redirects(&(*hashtable)[i]);
		i++;
	}
	return (1);
}
