#include "../../includes/minishell.h"

static void	handle_expand_fail(t_tkn **node)
{
	t_tkn	*temp;

	temp = *node;
	while (temp->prev)
		temp = temp->prev;
	if (ft_strcmp(temp->content, "echo") == 0)
	{
		free((*node)->content);
		temp  = (*node)->prev;
		temp->next = (*node)->next;
		if ((*node)->next)
			(*node)->next->prev = temp;
		if (temp->space_after != (*node)->space_after)
			temp->space_after = (*node)->space_after;
		free(*node);
		*node = temp;
	}
	if ((*node)->prev && (ft_strcmp((*node)->prev->content, ">") == 0
		|| ft_strcmp((*node)->prev->content, "<") == 0))
		ft_printf("\n%sminishell: %s: ambiguous redirect%s\n", RED, (*node)->content, END);
}

static int	get_var_value(t_global **data, char **line, int i)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (!is_special_var_char(line[i + len]))
		len++;
	if (is_special_variable(*line))
	{
		printf("%s\nThis functionality is beyond Minishell's scope.\n\n%s", RED, END);
		return (0);
	}
	value = search_value(data, *line, i, len);
	if (!value)
		handle_expand_fail(*line, i); // troca a var por "string vazia"
	else
		update_line(*line, i + len, len, &value);
	return (1);
}

void	expand_heredoc(t_global **data, char **line)
{
	int		i;
	int		result;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
		{
			i++;
			result = get_var_value(data, line, i);
			break ;
		}
		i++;
	}
}