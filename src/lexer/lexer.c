#include "../includes/minishell.h"

int	check_valid_input(t_tkn **node, int i)
{
	if (!input_starts_with_command((*node), i))
	{
		printf("%s: command not found\n", (*node)->content);
		return (0);
	}
	if ((is_double_special_token((*node)) && !(*node)->delimiter)
		|| (!(*node)->next && !ft_strcmp((*node)->type, SPECIAL_CHAR) && !(*node)->delimiter)
		|| is_and_or((*node)->content))
	{
		printf("Syntax error.\n");
		return (0);
	}
	return (1);
}

void	check_redirects(t_tkn **node)
{
	//t_tkn	*temp_node;
	t_tkn	*temp;

	temp = *node;
	//temp_node = NULL;
	while (*node)
	{
		parse_redir(node, &temp);
		*node = (*node)->next;
	}
	// if (temp_node && !temp_node->prev)
	// 	*node = temp_node;
	// else
	// {
	// 	*node = temp;
	// 	while ((*node)->next != NULL)
	// 		*node = (*node)->next;
	// 	*node = temp_node;
		*node = temp;
	//}
}

int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!check_valid_input(&(*hashtable)[i], i))
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
