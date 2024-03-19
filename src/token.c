#include "../includes/minishell.h"

void	prt_hashtable(t_tkn *hashtable[TABLE_SIZE])
{
	int	i;

	i = 0;
	while (hashtable[i])
	{
		while (hashtable[i])
		{
			printf("%s%s%s ", YELLOW, hashtable[i]->content, END);
			hashtable[i] = hashtable[i]->next;
		}
		i++;
	}
	printf("\n");
}

int	check_quotes(char *input, int i)
{
	int		len;
	char	start;

	len = 0;
	if (input[i] == SIMPLE_QUOTE)
		start = SIMPLE_QUOTE;
	if (input[i] == DOUBLE_QUOTE)
		start = DOUBLE_QUOTE;
	i++;
	len++;
	while (input[i] && start != input[i])
	{
		len++;
		i++;
	}
	if (!input[i])
		return (-1);
	if (input[i] == start)
		return (len + 1);
	return (0);
}

int	get_token_len(char *input, int i)
{
	int	len;

	len = 0;
	if (input[i] == '|')
		len++;
	else if (input[i] == SIMPLE_QUOTE || input[i] == DOUBLE_QUOTE)
		len = check_quotes(input, i);
	else
	{
		while (input[i + len] && input[i + len] != ' '
			&& input[i + len] != SIMPLE_QUOTE && input[i + len] != DOUBLE_QUOTE)
		{
			len++;
		}
	}
	return (len);
}

int	handle_input(t_global **data)
{
	int		i;
	int		len;

	add_history((*data)->usr_input);
	if (check_exit_input(&(*data)->usr_input, &(*data)->exit))
		return (-1);
	i = 0;
	while ((*data)->usr_input[i])
	{
		while ((*data)->usr_input[i] == ' ')
			i++;
		len = get_token_len((*data)->usr_input, i);
		if (len == -1)
		{
			printf("Error: open quote.\n");
			return (0);
		}
		populate_hashtable(data, i, len);
		i += len;
	}
	return (1);
}

int	readline_and_handle_input(t_global **data)
{
	int	input;

	(*data)->usr_input = NULL;
	(*data)->usr_input = readline((*data)->usr_input);
	input = handle_input(data);
	if (input == -1)
		return (-1);
	if (input == 1)
	{
		prt_hashtable((*data)->hashtable);
		parse(&(*data)->hashtable);
	}
	return (1);
}
