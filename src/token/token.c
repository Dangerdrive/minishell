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

/**
 * Check if the 'input' parameter is validly quoted (starts and ends with quotes).
 *
 * Returns the input length (with quotes) if it is validly quoted.
 * Returns (-1) if it is NOT validly quoted.
 * Otherwise, returns (0).
 */
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

/**
 * Check if the 'c' parameter is a special bash char (|, &, <, >).
 *
 * Returns true or false.
 */
t_bool	is_special_char(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>')
		return (true);
	return (false);
}

/**
 * Counts the length of the 'input'(char *) parameter, starting on the 'i'(index).
 *
 * Returns the lenght of the token started in the position 'input[i]'.
 */
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
		if (input[i + len] == '$')
			len++;
		while (input[i + len] && input[i + len] != ' ' && input[i + len] != '$'
		&& !is_special_char(input[i + len]) && input[i + len] != SIMPLE_QUOTE
		&& input[i + len] != DOUBLE_QUOTE)
			len++;
	}
	return (len);
}

/**
 * Handles the usr_input, tokenizing them in a hashtable.
 *
 * Returns (1) if there is no problem during tokenization.
 * Returns (0) if an open quote is identified.
 * Returns (-1) if usr_input == "exit".
 */
int	handle_input(t_global **data)
{
	int		i;
	int		len;

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

	(*data)->original_stdin = dup(STDIN_FILENO);
	(*data)->original_stdout = dup(STDOUT_FILENO);	

	input = 0;
	(*data)->usr_input = NULL;
	(*data)->usr_input = readline((*data)->usr_input);
	// if (!(*data)->usr_input) // EOF CATCHER (CTRL-D)
	// 	return (-1);
	add_history((*data)->usr_input);
	input = handle_input(data);
	if (input == -1)
		return (-1);
	if (input == 1)
	{
		input = parse(&(*data)->hashtable, data);
		 if (input == 1)
		 {
			prepare_exec(*data);
			// 	prt_hashtable((*data)->hashtable);
		 }
	}
	return (1);
}
