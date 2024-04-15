#include "../includes/minishell.h"

t_bool	is_redir_in(char *c)
{
	if (!ft_strcmp(c, LESS_THAN))
		return (true);
	return (false);
}

t_bool	is_redir_out(char *c)
{
	if (!ft_strcmp(c, GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_append(char *c)
{
	if (!ft_strcmp(c, DOUBLE_GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_heredoc(char *content)
{
	if (strcmp(content, DOUBLE_LESS_THAN) == 0)
		return (true);
	return (false);
}

t_bool	is_redir(char *sig)
{
	if (is_redir_in(sig) || is_redir_out(sig)
		|| is_heredoc(sig) || is_append(sig))
		return (true);
	return (false);
}
