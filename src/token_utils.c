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
	token[len] = '\0';
	while (--len >= 0)
		token[len] = input[i + len];
	return (token);
}

void	add_node(t_tkn **tkn_node, char *input, int i, int len)
{
	t_tkn	*new_node;
	t_tkn	*temp;

	new_node = ft_calloc(1, sizeof(t_tkn));
	temp = NULL;
	if (!new_node)
		return ;
	new_node->content = get_token(input, i, len);
	new_node->next = NULL;
	if (!(*tkn_node))
		(*tkn_node) = new_node;
	else
	{
		temp = (*tkn_node);
		while ((*tkn_node)->next != NULL)
			(*tkn_node) = (*tkn_node)->next;
		(*tkn_node)->next = new_node;
		printf("%s%s%s ", YELLOW, (*tkn_node)->next->content, END);
		*tkn_node = temp;
	}
}

void	populate_hashtable(t_global ***data, int i, int len)
{
	add_node(&(**data)->hashtable[0], (**data)->usr_input, i, len);
	//printf("hash: %s%s%s\n", YELLOW, ((*hashtable)[0])->content, END);
}

int	check_exit_input(char **input, int *exit)
{
	if (ft_strncmp(*input, "exit", 5) == 0)
	{
		*exit = 1;
		return (1);
	}
	return (0);
}
