#include "../../includes/minishell.h"

void	exec_nonbuiltin(char **args, t_global *data)
{
	char	*cmd;

	cmd = get_cmd(args[0], data);
	if (cmd)
	{
		if (execve(cmd, args, data->env) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", args[0], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", args[0]);
		rl_clear_history();
		//freeenv?
		ft_strarr_free(args, ft_strarr_len(args));
		close_all_fds();	
		
		exit(127);//command not found.
	}
}

int	exec_one_nonbuiltin(t_global *data, char **args)
{
	pid_t	pid;
	int		ret;
	int		status;

	ret = 1;
	pid = fork();
	//define_execute_signals(child_pid); lidar com sinais

	if (pid == -1)
		perror("minishell: fork");
	else if (pid == 0)
	{
		exec_nonbuiltin(args, data);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
        	ft_dprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
		//	("waitpid", ft_itoa(pid));
		// lidar com sinal
        // else if (WIFSIGNALED(status))
        //     ret = handle_signal_interrupt(status, TRUE);
        else if (WIFEXITED(status))
            ret = WEXITSTATUS(status);
    }
	return(ret); 
}

int	exec_one_process(t_global *data)
{
	int		ret;
	char	**args;

	ret = 1;
	args = NULL;
	if (handle_redirects(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (1);
	}
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (args && args[0] && is_builtin(args[0]))
		ret = exec_builtin(args, hashsize(data->hashtable[0]), data);
	else if (args && args[0] && !is_builtin(args[0]))
		ret = exec_one_nonbuiltin(data, args);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	restore_original_fds(data->original_fds);
	return (ret);
}
