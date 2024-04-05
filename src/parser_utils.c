#include "../includes/minishell.h"

t_bool	is_pipe(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0))
		return (true);
	return (false);
}

t_bool	is_and_or(char *token)
{
	if (token && (ft_strcmp(token, LOGIC_AND) == 0
		|| ft_strcmp(token, LOGIC_OR) == 0))
		return (true);
	return (false);
}

t_bool	is_pipe_and_or(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0
		|| ft_strcmp(token, LOGIC_AND) == 0
		|| ft_strcmp(token, LOGIC_OR) == 0))
		return (true);
	return (false);
}

t_bool	is_special_token(char *token)
{
	if (token
		&& ((ft_strncmp(token, PIPE, 1) == 0)
			|| (ft_strncmp(token, AMPERSAND, 1) == 0)
			|| (ft_strncmp(token, GREATER_THAN, 1) == 0)
			|| (ft_strncmp(token, LESS_THAN, 1) == 0)
			|| (ft_strncmp(token, LOGIC_OR, 2) == 0)
			|| (ft_strncmp(token, LOGIC_AND, 2) == 0)
			|| (ft_strncmp(token, DOUBLE_GREATER_THAN, 2) == 0)
			|| (ft_strncmp(token, DOUBLE_LESS_THAN, 2) == 0)))
		return (true);
	else
		return (false);
}
