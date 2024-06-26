#include "../../includes/minishell.h"

// static void	handle_expand_fail(char **line, int i, int len)
// {
// 	char	*new_line;
// 	int		j;

// 	j = 0;
// 	new_line = ft_calloc((ft_strlen(*line) - len + 1), sizeof(char));
// 	while ((*line)[j])
// 	{
// 		new_line[j] = (*line)[j];
// 		j++;
// 		if (j == i)
// 			j += len;
// 	}
// 	free(*line);
// 	*line = ft_strdup(new_line);
// 	free(new_line);
// }

static void	update_line(char **line, int len, char **value)
{
	char	*new_line;
	char	*temp;
	int		line_len;

	line_len = 0;
	temp = NULL;
	while((*line)[line_len] != '$')
		line_len++;
	new_line = ft_strndup((*line), line_len);
	if ((*line)[line_len] == '$')
	{
		if (*value)
			temp = ft_strjoin(new_line, *value);
		else
			temp = ft_strdup(new_line);
		free(new_line);
		new_line = ft_strjoin(temp, (*line + line_len + len + 1));
	}
	free(temp);
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
	update_line(line, len, &value);
	return (1);
}

void	expand_heredoc(t_global *data, char **line)
{
	int		i;

	i = 0;
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == '$')
		{
			i++;
			get_var_value(&data, line, i);
			break ;
		}
		i++;
	}
}