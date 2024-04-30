#include "../includes/minishell.h"

t_bool	is_special_var_char(char c)
{
	if ((c >= 32 && c <= 47 && c != 35) || (c >= 58 && c <= 64 && c != 63)
		|| (c >= 93 && c <= 96 && c != '_') || (c >= 123 && c <= 126))
	{
		return (true);
	}
	return (false);
}

t_bool	is_special_variable(char *var, t_bool print)
{
	if (!ft_strcmp(var, "$#") || !ft_strcmp(var, "$!") || !ft_strcmp(var, "$@")
		|| !ft_strcmp(var, "$$") || !ft_strcmp(var, "$0")
		|| !ft_strcmp(var, "$-") || !ft_strcmp(var, "$*"))
	{
		if (print)
			ft_dprintf(2, "%s\nThis functionality is beyond Minishell's \
			scope, ****@#$@***.\n\n%s", RED, END);
		return (true);
	}
	return (false);
}

t_bool	is_var_name(t_tkn **node, int i, int len)
{
	if (((*node)->type[0] == 'v' && (*node)->content[i + len]
			&& !is_special_var_char((*node)->content[i + len]))
		|| ((*node)->type[0] == 's' && (*node)->content[i + len]
			&& !is_special_var_char((*node)->content[i + len])))
		return (TRUE);
	if (!ft_strcmp((*node)->type, EXPT_VARIABLE) && (*node)->content[i + len]
		&& !is_special_var_char((*node)->content[i + len]))
		return (TRUE);
	return (FALSE);
}

void	add_node_before(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return ;
	new_node->content = ft_strndup((*node)->content, i - 1);
	new_node->type = STRING_STD;
	new_node->next = (*node);
	new_node->prev = (*node)->prev;
	(*node)->prev->next = new_node;
}

t_tkn	*add_node_after(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup((*node)->content + i);
	new_node->type = STRING_STD;
	new_node->next = (*node)->next;
	new_node->prev = *node;
	if ((*node)->next)
		(*node)->next->prev = new_node;
	return (new_node);
}
