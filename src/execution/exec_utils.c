#include "../../includes/minishell.h"

/**
 * Calculates the number of tokens in a linked list representing a command hash table.
 * 
 * @param[in] hashtable Pointer to the head of the token linked list.
 * @return The count of tokens or -1 if the hashtable is NULL.
 */
int	hashsize(t_tkn *hashtable)
{
	int		i;
	t_tkn	*temp;

	if (!hashtable)
		return (-1);
	temp = hashtable;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

/**
 * Checks if the current node should be considered an empty string based on 
 * specific conditions related to surrounding tokens.
 * 
 * @param[in] node Double pointer to the current token node.
 * @return TRUE if conditions are met to treat as an empty string, 
 * FALSE otherwise.
 */
t_bool	check_empty_str(t_tkn **node)
{
	if ((*node)->prev && ((*node)->prev->content
			&& !ft_strcmp((*node)->prev->content, "")
			&& (*node)->prev->space_after == TRUE)
		&& (*node)->prev->prev && (*node)->prev->prev->space_after == TRUE)
		return (TRUE);
	return (FALSE);
}

/**
 * Fills an array of arguments by joining tokens based on their spacing.
 * 
 * @param[in, out] node Pointer to the current node being processed.
 * @param[out] args Array to fill with arguments.
 * @param[in] i Index at which to insert the new argument.
 */
void	fill_args(t_tkn	**node, char **args, int i)
{
	char	*arg_tmp;

	arg_tmp = NULL;
	if (check_empty_str(node))
		args[i] = ft_strjoin(" ", (*node)->content);
	else if (!(*node)->prev
		|| ((*node)->prev && ((*node)->prev->space_after == TRUE)
			&& (*node)->content))
		args[i] = ft_strdup((*node)->content);
	while (((*node)->space_after == FALSE) && (*node)->next)
	{
		if (!ft_strcmp((*node)->next->content, "") && (*node)->next->next
			&& (*node)->next->space_after == FALSE)
		{
			arg_tmp = ft_strjoin(args[i], (*node)->next->next->content);
			*node = (*node)->next;
		}
		else
			arg_tmp = ft_strjoin(args[i], (*node)->next->content);
		free(args[i]);
		args[i] = arg_tmp;
		(*node) = (*node)->next;
	}
}

/**
 * Converts a hash table of token nodes into an array of strings.
 * 
 * @param[in] node Head of the token linked list.
 * @return Array of strings constructed from the hash table.
 */
char	**hash_to_args(t_tkn *node)
{
	char	**args;
	t_tkn	*temp;
	int		i;
	int		args_count;

	if (!node->content && !node->next)
		return (NULL);
	args_count = hashsize(node);
	args = ft_calloc((args_count + 1), sizeof(char *));
	temp = node;
	i = 0;
	while (temp)
	{
		fill_args(&temp, args, i);
		temp = temp->next;
		i++;
	}
	args[args_count] = NULL;
	return (args);
}

/**
 * Restores the original file descriptors for standard input and output.
 * 
 * @param[in] original_fds Array containing the original file descriptors
 *  to restore.
 */
void	restore_fds(int original_fds[2])
{
	redirect_fd(original_fds[IN], STDIN_FILENO);
	redirect_fd(original_fds[OUT], STDOUT_FILENO);
}
