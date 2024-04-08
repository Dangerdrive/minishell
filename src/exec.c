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

char	**hash_to_args(t_tkn *hashtable)
{
	char	**args;
	t_tkn	*temp;
	int		i;
	int		args_count;

	args_count = hashsize(hashtable);
	args = malloc(sizeof(char *) * (args_count + 1));
	i = -1;
	temp = hashtable;
	while (temp)
	{
		args[++i] = ft_strdup(temp->content);
		temp = temp->next;
	}
	args[++i] = NULL;
	// //debug
	// while (i >= 0)
	// {
	// printf("%i: %s\n", i, args[i]);
	// 	i--;
	// }
	return (args);
}	

int	prepare_exec(t_global *data)
{
	char	**args;
	// int		i;

	// i = 0;
	args = hash_to_args(data->hashtable[0]);
	if (is_builtin(args[0]))
		return (exec_builtin(args, hashsize(data->hashtable[0]), data));
	// else
	// 	return (exec(data, args));
	if (args)
		ft_strarr_free(args, hashsize(data->hashtable[0]));
	return (0);
}

// int	exec(t_global *data, char **args)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (execve(args[0], args, data->env) == -1)
// 		{
// 			ft_printf_fd(2, "minishell: %s: %s\n", args[0], strerror(errno));
// 			exit(1);
// 		}
// 	}
// 	else if (pid < 0)
// 	{
// 		ft_printf_fd(2, "minishell: %s\n", strerror(errno));
// 		return (1);
// 	}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			data->ret = WEXITSTATUS(status);
// 	}
// 	return (0);
// }
