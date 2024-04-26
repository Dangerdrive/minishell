#include "includes/minishell.h"

static void	update_line(char **line, int len, char *value)
{
	char	*new_line;
	char	*temp;
	int		line_len;

	line_len = 0;
	while(*line[line_len] && *line[line_len] != '$')
		line_len++;
	new_line = ft_strndup(*line, line_len);
	if (*line[line_len] == '$')
	{
		temp = ft_strjoin(new_line, value);
		free(new_line);
		new_line = ft_strjoin(temp, (*line + line_len + len + 1));
	}
	free(*line);
	free(value);
	*line = ft_strdup(new_line);
	free(new_line);
}

int	main(int argc, char **argv)
{
	char *line;

	line = ft_strdup(argv[1]);
	update_line(&line, ft_strlen(argv[1]), "lalalalala");
	printf("Line = %s\n", line);
	return (0);
}
