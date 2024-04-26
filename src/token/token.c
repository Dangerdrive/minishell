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

int	handle_input(t_global **data)
{
	int		i;
	int		len;

	if (check_exit_input(&(*data)->usr_input, *data)) // só deve ser chamado se não houver pipe
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
		if (len > 0)
			populate_hashtable(data, i, len);
		i += len;
	}
	return (1);
}

char	*cwd_error(void)
{
	static char	prompt[PATH_MAX];

	ft_bzero(prompt, sizeof(prompt));
	ft_strlcat(prompt, "minishell: ", PATH_MAX);
	ft_strlcat(prompt, strerror(errno), PATH_MAX);
	return (prompt);
}

char *print_cwd(t_global **data)
{
	static char	prompt[PATH_MAX];
	char	*cwd;
	int		home;

	cwd = getcwd(NULL, 0);
	ft_bzero(prompt, sizeof(prompt));
	if (!cwd)
		return (cwd_error());
	home = ft_strlen(ft_getenv("HOME", data));
	if (ft_strncmp(cwd, ft_getenv("HOME", data), home) == 0)
	{
		ft_strlcat(prompt, BLUE, PATH_MAX);
		ft_strlcat(prompt, (*data)->usr, PATH_MAX);
		ft_strlcat(prompt, ":~", PATH_MAX);
		ft_strlcat(prompt, &cwd[home], PATH_MAX);
		ft_strlcat(prompt, "$ ", PATH_MAX);
		ft_strlcat(prompt, END, PATH_MAX);
	}
	else
		ft_strlcat(prompt, cwd, PATH_MAX);
	free(cwd);
	return (prompt);
}

int	readline_and_handle_input(t_global **data)
{
	int	input;

	input = 0;
	(*data)->usr_input = NULL;
	(*data)->usr_input = readline(print_cwd(data));
	if (!(*data)->usr_input)
		return (-1);
	if (!(*data)->usr_input[0])
	{
		//printf("\n");
		return (0);
	}
	add_history((*data)->usr_input);
	input = handle_input(data);
	if (input == -1)
		return (-1);
	if (input == 1)
	{
		input = parse(&(*data)->hashtable, data);
		if (input == 1)
			(*data)->ret = handle_execution(*data);
		// 	prt_hashtable((*data)->hashtable);
	}
	return (1);
}
