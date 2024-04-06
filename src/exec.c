#include "../../includes/minishell.h"

int	hashsize(t_tkn *hashtable)
{
	int		i;
	t_tkn	*temp;

	temp = hashtable;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char	**hash_to_args(t_global *data)
{
	char	**args;
	t_tkn	*temp;
	int		i;
	int		args_count;

	args_count = hashsize(data->hashtable[0]);
	args = malloc(sizeof(char *) * (args_count + 1));
	i = -1;
	temp = data->hashtable[0];
	while (temp)
	{
		args[i++] = ft_strdup(temp->content);
		temp = temp->next;
	}
	args[i] = NULL;
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
}
