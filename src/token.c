#include "../includes/minishell.h"

int	check_quotes(char *input, int i) //TRATAR ASPAS TBM ENTRE LETRAS (ALÉM DE ESPAÇOS)
{
	int		len;
	char	start;

	len = 0;
	start = 0;
	if (input[i] == SIMPLE_QUOTE)
		start = SIMPLE_QUOTE;
	if (input[i] == DOUBLE_QUOTE)
		start = DOUBLE_QUOTE;
	while ((start == 0 && input[i] && input[i] != ' ')
		|| (start != 0 && start != input[i]))
	{
		len++;
		if (start == 0 && (input[i] == SIMPLE_QUOTE || input[i] == DOUBLE_QUOTE))
			return (-1);
		i++;
	}
	if (input[i] == start)
		return (len);
	return (0);
}

// int	check_quotes(char *input, int i) //TRATAR ASPAS NO FINAL DO INPUT
// {
// 	int		len;
// 	int		is_simple;
// 	int		is_double;

// 	if (input[i] == SIMPLE_QUOTE || input[i] == DOUBLE_QUOTE)
// 	{
// 		if (input[i] == SIMPLE_QUOTE)
// 			is_simple = 1;
// 		else
// 			is_double = 1;
// 		i++;
// 		len = 1;
// 		while (input[i])
// 		{
// 			len++;
// 			if (input[i] == SIMPLE_QUOTE || input[i] == DOUBLE_QUOTE)
// 			{
// 				if (is_simple == 1 && input[i] == SIMPLE_QUOTE)
// 					is_simple++;
// 				if (is_double == 1 && input[i] == DOUBLE_QUOTE)
// 					is_double++;
// 				break ;
// 			}
// 			i++;
// 		}
// 		//printf("is_simple = %d\n", is_simple);
// 		if (is_simple == 2 || is_double == 2)
// 			return (len);
// 		else
// 			return (-1);
// 	}
// 	return (0);
// }

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
