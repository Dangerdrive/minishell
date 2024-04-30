#include "../includes/minishell.h"

/**
 * Checks for closed quotes starting from a given index in a string.
 *
 * @param[in] input The input string to check.
 * @param[in] i The index to start checking from.
 * @return Length of the quoted segment if properly closed, -1 if not closed,
 *         0 if no quotes are found.
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
 * Determines if a character is considered special character for tokenization.
 *
 * @param[in] c The character to evaluate.
 * @return True if the character is special ('|', '&', '<', '>'),
 *  false otherwise.
 */
t_bool	is_special_char(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>')
		return (true);
	return (false);
}

/**
 * Calculates the length of the token starting from the specified index.
 *
 * @param[in] input The string to parse.
 * @param[in] i The start index for token measurement.
 * @return The length of the token.
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
			&& !is_special_char(input[i + len])
			&& input[i + len] != SIMPLE_QUOTE
			&& input[i + len] != DOUBLE_QUOTE)
			len++;
	}
	return (len);
}

int	error_open_quote(void)
{
	printf("Error: open quote.\n");
	return (0);
}

/**
 * Tokenizes the user input stored in global data.
 *
 * @param[in, out] data The global data structure containing user input.
 * @return 1 on successful tokenization, 0 on error (e.g., open quote).
 */
int	tokenize(t_global **data)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	if (!ft_strcmp((*data)->usr_input, " "))
		populate_hashtable(data, 0, 1);
	else
	{
		while ((*data)->usr_input[i])
		{
			while ((*data)->usr_input[i] == ' ')
				i++;
			len = get_token_len((*data)->usr_input, i);
			if (len == -1)
				return (error_open_quote());
			if (len > 0)
				populate_hashtable(data, i, len);
			i += len;
		}
	}
	if (len == 0)
		return (0);
	return (1);
}
