/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:59:41 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:59:43 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_line(char **line, int len, char **value)
{
	char	*new_line;
	char	*temp;
	int		line_len;

	line_len = 0;
	temp = NULL;
	while ((*line)[line_len] != '$')
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

int	get_var_value1(t_global **data, char **line, int i)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while ((*line)[i + len] && !is_special_var_char((*line)[i + len]))
		len++;
	if (((*line)[i] == '?' && !(*line)[i + 1])
			|| !ft_strncmp((*line) + i, "? ", 2))
		value = ft_itoa((*data)->ret);
	else if (is_special_variable(*line, FALSE))
	{
		printf("%s\nThis functionality is beyond \
		Minishell's scope.\n\n%s", RED, END);
		return (0);
	}
	else
		value = search_value(data, line, i, len);
	update_line(line, len, &value);
	return (1);
}

void	expand_heredoc(t_global *data, char **line)
{
	int		i;

	i = 0;
	while (i < (int)ft_strlen(*line))
	{
		if ((*line)[i] == '$')
		{
			i++;
			get_var_value1(&data, line, i);
		}
		i++;
	}
}
