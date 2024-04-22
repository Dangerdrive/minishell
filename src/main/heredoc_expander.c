#include "../../includes/minishell.h"

static void	handle_expand_fail(char **line, int i, int len, char **value)
{
	char	*new_line;
	int		j;

	j = 0;
	new_line = ft_calloc((ft_strlen(*line) - len + 1), sizeof(char));
	while (line[j])
	{
		new_line[j] = line[j];
		j++;
		if (j == i)
			j == len;
	}
	free(*line);
	line = ft_strdup(new_line);
	free(new_line);
}

static int	get_var_value(t_global **data, char **line, int i)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (!is_special_var_char((*line)[i + len]))
		len++;
	if (is_special_variable(*line))
	{
		printf("%s\nThis functionality is beyond Minishell's scope.\n\n%s", RED, END);
		return (0);
	}
	value = search_value(data, line, i, len);
	if (!value)
		handle_expand_fail(line, i, len, value); // troca a var por "string vazia"
	// else
	// 	update_line(*line, i + len, len, &value);
	return (1);
}

void	expand_heredoc(t_global **data, char *line)
{
	int		i;
	//int		result;

	i = 0;
	//result = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{
			i++;
			//result =
			get_var_value(data, &line, i);
			break ;
		}
		i++;
	}
}