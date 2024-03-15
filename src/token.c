#include "../includes/minishell.h"

int	check_quotes(char *input, int i) //TRATAR ASPAS NO FINAL DO INPUT
{
	int		len;
	int		is_simple;

	is_simple = 0;
	if (input[i] == SIMPLE_QUOTE || input[i] == DOUBLE_QUOTE)
	{
		if (input[i] == SIMPLE_QUOTE)
			is_simple = 1;
		i++;
		len = 1;
		while (input[i])
		{
			if ((is_simple == 1 && input[i] == SIMPLE_QUOTE)
				|| (is_simple == 0 && input[i] == DOUBLE_QUOTE))
				return (len + 1);
			i++;
			len++;
		}
		return (-1);
	}
	return (0);
}

// int	is_reserved_char(char *input, int i)
// {
// 	if (ft_strchr(input))
// 	{

// 	}
// }

int	get_token_len(char *input, int i)
{
	int	len;

	len = 0;
	while (input[i + len] != ' ' && input[i + len] != '\0')
	{
		len++;
	}
	return (len);
}

int	handle_input(t_global **data)
{
	int		i;
	int		len;

	(*data)->usr_input = NULL;
	(*data)->usr_input = readline((*data)->usr_input);
	add_history((*data)->usr_input);
	if (check_exit_input(&(*data)->usr_input, &(*data)->exit))
		return (-1);
	i = 0;
	while ((*data)->usr_input[i])
	{
		while ((*data)->usr_input[i] == ' ')
			i++;
		len = check_quotes((*data)->usr_input, i);
		if (len == -1)
		{
			printf("Error: open quote.\n");
			return (0);
		}
		else if (len == 0)
			len = get_token_len((*data)->usr_input, i);
		populate_hashtable(&data, i, len);
		i += len;
	}
	printf("\n");
	return (1);
}
