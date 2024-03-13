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

	new_node = ft_calloc(1, sizeof(t_tkn));
	temp = NULL;
	if (!new_node)
		return ;
	new_node->content = token;
	new_node->next = NULL;
	if (!(*tkn_node))
		*tkn_node = new_node;
	else
	{
		temp = *tkn_node;
		while ((*tkn_node)->next != NULL)
			(*tkn_node) = (*tkn_node)->next;
		(*tkn_node)->next = new_node;
		*tkn_node = temp;
	}
}

void	populate_hashtable(t_tkn *(*hashtable)[TABLE_SIZE], char *token)
{
	add_node(&(*hashtable)[0], token);
	printf("hash: %s%s%s\n", YELLOW, ((*hashtable)[0])->content, END);
}

int	check_exit_input(char **input)
{
	if (ft_strncmp(*input, "exit", 5) == 0)
	{
		free(*input);
		return (1);
	}
	return (0);
}

char	*save_token(char *input, int i, int len)
{
	char	*token;

	token = ft_calloc(len + 1, sizeof(char));
	token[len] = '\0';
	while (--len >= 0)
		token[len] = input[i + len];
	return (token);
}

int	check_quotes(char *input, int i)
{
	int		len;

	if (input[i] == 34)
	{
		i++;
		len = 1;
		while (input[i])
		{
			if (input[i] == 34)
				return (len + 1);
			i++;
			len++;
		}
		return (-1);
	}
	return (0);
}

int	get_token_len(char *input, int i)
{
	int	len;

	len = 0;
	while (input[i + len] != ' ' && input[i + len] != '\0'
		&& input[i + len] != 34)
		len++;
	return (len);
}

int	handle_input(t_global **data)
{
	int		i;
	int		len;
	char	*token;

	// if ((*data)->usr_input != NULL)
	// 	ft_memdel((*data)->usr_input);
	(*data)->usr_input = NULL;
	(*data)->usr_input = readline((*data)->usr_input);
	add_history((*data)->usr_input);
	if (check_exit_input(&(*data)->usr_input))
		return (0);
	i = 0;
	while ((*data)->usr_input[i])
	{
		while ((*data)->usr_input[i] == ' ')
			i++;
		len = check_quotes((*data)->usr_input, i);
		if (len == -1)
			return (0);
		else if (len == 0)
			len = get_token_len((*data)->usr_input, i);
		token = save_token((*data)->usr_input, i, len);
		populate_hashtable(&(*data)->hashtable, token);
		free(token);
		i += len;
	}
	return (1);
}
