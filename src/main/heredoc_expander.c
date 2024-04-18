#include "../../includes/minishell.h"

// static char	*search_value(t_global **data, t_tkn **node, int i, int len)
// {
// 	char	*value;
// 	int		j;

// 	value = NULL;
// 	j = 0;
// 	while (!value && (*data)->env[j])
// 	{
// 		value = fetch_in_array(node, i, len, (*data)->env[j]);
// 		j++;
// 	}
// 	j = 0;
// 	if ((*data)->exported)
// 	{
// 		while (!value && (*data)->exported[j])
// 		{
// 			value = fetch_in_array(node, i, len, (*data)->exported[j]);
// 			j++;
// 		}
// 	}
// 	return (value);
// }

// t_bool	is_var_name(char **line, int i, int len)
// {
// 	if (((*node)->type[0] == 'v' && (*node)->content[i + len]
// 		&& !is_special_var_char((*node)->content[i + len]))
// 		|| ((*node)->type[0] == 's' && (*node)->content[i + len]
// 			&& !is_special_var_char((*node)->content[i + len])))
// 		return (true);
// 	return (false);
// }

// static int	get_var_value(t_global **data, char **line, int i)
// {
// 	int		len;
// 	char	*value;

// 	len = 0;
// 	value = NULL;
// 	while (!is_special_var_char(line[i + len]))
// 		len++;
// 	if (is_special_variable(*line))
// 	{
// 		printf("%s\nThis functionality is beyond Minishell's scope.\n\n%s", RED, END);
// 		return (0);
// 	}
// 	value = search_value(data, *line, i, len);
// 	if (!value)
// 		handle_expand_fail(*line);
// 	else
// 		update_line(*line, i + len, len, &value);
// 	return (1);
// }

// void	expand_heredoc(t_global **data, char **line)
// {
// 	int		i;
// 	int		result;

// 	i = 0;
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '$')
// 		{
// 			i++;
// 			result = get_var_value(data, line, i);
// 			break ;
// 		}
// 		i++;
// 	}
// }