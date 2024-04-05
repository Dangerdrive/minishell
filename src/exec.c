#include "../../includes/minishell.h"



int	exec(t_global *data, char **args)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(args[0], args, data->env) == -1)
		{
			ft_printf_fd(2, "minishell: %s: %s\n", args[0], strerror(errno));
			exit(1);
		}
	}
	else if (pid < 0)
	{
		ft_printf_fd(2, "minishell: %s\n", strerror(errno));
		return (1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->ret = WEXITSTATUS(status);
	}
	return (0);
}
