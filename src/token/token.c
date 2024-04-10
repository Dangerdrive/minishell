#include "../includes/minishell.h"

void	prt_hashtable(t_tkn *hashtable[TABLE_SIZE])
{
	int		i;
	t_tkn	*temp;

	i = 0;
	while (hashtable[i])
	{
		temp = hashtable[i];
		while (hashtable[i])
		{
			// printf("'%s%s%s'	-	type: %s\n", YELLOW, hashtable[i]->content, END,
			// 	hashtable[i]->type);
			hashtable[i] = hashtable[i]->next;
		}
		hashtable[i] = temp;
		i++;
	}
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

bool	is_special_char(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>')
		return (true);
	return (false);
}

int	get_token_len(char *input, int i)
{
	int	len;

	len = 0;
	if (is_special_char(input[i]))
	{
		len++;
		if (input[i + 1] && input[i + 1] == input[i])
			len++;
	}
	else if (input[i] == SIMPLE_QUOTE || input[i] == DOUBLE_QUOTE)
		len = check_quotes(input, i);
	else
	{
		while (input[i + len] && input[i + len] != ' ' && !is_special_char(input[i + len])
			&& input[i + len] != SIMPLE_QUOTE && input[i + len] != DOUBLE_QUOTE)
			len++;
	}
	return (len);
}

int	handle_input(t_global **data)
{
	int		i;
	int		len;

	if (check_exit_input(&(*data)->usr_input, *data)) // só deve ser chamado se não houver pipe
		return (-1);
	i = 0;
	len = 1;
	while ((*data)->usr_input[i] && len > 0)
	{
		len = 0;
		while ((*data)->usr_input[i] == ' ')
			i++;
		len = get_token_len((*data)->usr_input, i);
		if (len == -1)
		{
			printf("Error: open quote.\n");
			return (0);
		}
		if (len > 0)
			populate_hashtable(data, i, len);
		i += len;
	}
	return (1);
}

int	readline_and_handle_input(t_global **data)
{
	int	input;

	input = 0;
	handle_signals(data);
	(*data)->usr_input = NULL;
	(*data)->usr_input = readline((*data)->usr_input);
	if (!(*data)->usr_input) // EOF CATCHER (CTRL-D)
		return (-1);
	add_history((*data)->usr_input);
	input = handle_input(data);
	if (input == -1)
		return (-1);
	if (input == 1)
	{
		input = parse(&(*data)->hashtable, data);
		prepare_exec(*data);
		// if (input == 1)
		// 	prt_hashtable((*data)->hashtable);
	}
	return (1);
}
