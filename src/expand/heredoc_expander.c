#include "../../includes/minishell.h"

static void	handle_expand_fail(char **line, int i, int len)
{
	char	*new_line;
	int		j;

	j = 0;
	new_line = ft_calloc((ft_strlen(*line) - len + 1), sizeof(char));
	while (*line[j])
	{
		new_line[j] = *line[j];
		j++;
		if (j == i)
			j += len;
	}
	free(*line);
	*line = ft_strdup(new_line);
	free(new_line);
}

static void	update_line(char **line, int i, int len, char **value)
{
	char	*new_line;
	char	*temp;
	//int		total_len;
	//int		val_len;
	int		line_len;
	(void)i;
	// line_len = ft_strlen(*line);
	// val_len = ft_strlen(*value);
	// total_len = line_len - len + val_len;
	// new_line = ft_calloc((total_len + 1), sizeof(char));
	// ft_strlcpy(new_line, *line, i);
	// ft_strlcpy((new_line + i), *value, val_len);
	// ft_strlcpy((new_line + i + val_len), (*line + i + len), (line_len - i - len));

	line_len = 0;
	while(*line[line_len] && *line[line_len] != '$')
		line_len++;
	new_line = ft_strndup(*line, line_len);
	if (*line[line_len] == '$')
	{
		temp = ft_strjoin(new_line, *value);
		free(new_line);
		new_line = ft_strjoin(temp, (*line + line_len + len + 1));
	}
	free(*line);
	free(*value);
	*line = ft_strdup(new_line);
	free(new_line);
}

static int	get_var_value(t_global **data, char **line, int i)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while ((*line)[i + len] && !is_special_var_char((*line)[i + len]))
		len++;
	if (is_special_variable(*line))
	{
		printf("%s\nThis functionality is beyond Minishell's scope.\n\n%s", RED, END);
		return (0);
	}
	value = search_value(data, line, i, len);
	if (!value)
		handle_expand_fail(line, i, len);
	else
		update_line(line, i, len, &value);
	return (1);
}

void	expand_heredoc(t_global *data, char *line)
{
	int		i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{
			i++;
			get_var_value(&data, &line, i);
			break ;
		}
		i++;
	}
}