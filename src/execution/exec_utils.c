#include "../../includes/minishell.h"

int	hashsize(t_tkn *hashtable)
{
	int		i;
	t_tkn	*temp;

	if (!hashtable)
		return (-1);
	temp = hashtable;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

// REVISAR SITUAÇÃO STRING VAZIA (echo lala"" haha)
void	fill_args(t_tkn	**node, char **args, int i)
{
	char	*arg_tmp;

	arg_tmp = NULL;
	if ((*node)->prev && ((*node)->prev->content
			&& !ft_strcmp((*node)->prev->content, "")
			&& (*node)->prev->space_after == TRUE))
		args[i] = ft_strjoin(" ", (*node)->content);
	else if ((*node)->space_after == TRUE && (*node)->content)
		args[i] = ft_strdup((*node)->content);
	if ((*node)->space_after == FALSE && (*node)->next)
	{
		if ((*node)->prev->space_after == TRUE)
			args[i] = ft_strdup((*node)->content);
		arg_tmp = ft_strjoin(args[i], (*node)->next->content);
		free(args[i]);
		args[i] = arg_tmp;
		(*node) = (*node)->next;
	}
}

char	**hash_to_args(t_tkn *node)
{
	char	**args;
	t_tkn	*temp;
	int		i;
	int		args_count;

	if (!node->content && !node->next)
		return (NULL);
	args_count = hashsize(node);
	args = ft_calloc((args_count + 1), sizeof(char *));
	temp = node;
	i = 0;
	while (temp)
	{
		fill_args(&temp, args, i);
		temp = temp->next;
		i++;
	}
	args[args_count] = NULL;
	return (args);
}

void	restore_fds(int original_fds[2])
{
	redirect_fd(original_fds[IN], STDIN_FILENO);
	redirect_fd(original_fds[OUT], STDOUT_FILENO);
}
