#include "../../includes/minishell.h"

// /**
//  * Closes all file descriptors used in the pipex process.
//  *
//  * This function is responsible for closing the input and output file
//  * descriptors, as well as all pipe file descriptors associated with the pipex
//  * process. It checks if the input and output file descriptors are valid (not
//  * equal to -1) before attempting to close them. The function then calls
//  * `close_pipe_fds` to close all pipe file descriptors.
//  *
//  * @param[in,out] data Pointer to a t_data structure that contains the file
//  *				     descriptors for input, output, and pipes.
//  */
// void	close_fds(t_global *data)
//
//{
// 	int i;

// 	if (data->input_fd != -1)
// 		close(data->input_fd);
// 	if (data->output_fd != -1)
// 		close(data->output_fd);
// 	i = 0;
// 	while (i < (data->cmd_count - 1) * 2)
// 	{
// 		close(data->pipe[i]);
// 		i++;
// 	}
// }

// /**
//  * Creates pipe file descriptors for inter-process communication in pipex.
//  *
//  * This function is responsible for setting up the necessary pipes for the pipex
//  * program. It iterates through the required number of pipes, which is one less
//  * than the command count, and creates each pipe. The file descriptors for each
//  * pipe are stored in the 'pipe' field of the t_data structure. If a pipe
//  * creation fails, the function prints an error message and exits the program
//  * after performing cleanup.
//  *
//  * @param[in,out] data Pointer to a t_data structure containing the command
//  *                     count and the array to store pipe file descriptors.
//  */
// static int	create_pipes(t_global *data)
//
//{
// 	int	i;

// 	i = 0;
// 	while (i < hashsize(data->hashtable) - 1)
// 	{
// 		if (pipe(data->pipe + 2 * i) == -1)
// 		{
// 			ft_dprintf(2, "pipex: Could not create pipe: %s\n",
// 				strerror(errno));
// 			return (1);
// 			//cleanup_n_exit(ERROR, data);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// /**
//  * Initializes the t_data structure for the pipex program.
//  *
//  * This function initializes and sets up the t_data structure, which is used
//  * throughout the pipex program. It stores command line arguments, environment
//  * variables, and sets flags for 'here_doc' functionality. The function also
//  * allocates memory for storing process IDs (pids) and pipe file descriptors,
//  * handles input and output file setup, and creates the necessary pipes for
//  * inter-process communication.
//  *
//  * @param[in] ac The count of command line arguments.
//  * @param[in] av Array of command line argument strings.
//  * @param[in] envp Array of environment variable strings.
//  * @return An initialized t_data structure.
//  */
// t_data	init_data(int ac, char **av, char **envp)
//
// {
// 	t_data	data;

// 	data = initialize_data();

// 	get_input_file(&data);
// 	get_output_file(&data);
// 	hashsize() = ac - 3 - data.heredoc_flag;
// 	data.pids = malloc(sizeof(*data.pids) * hashsize());
// 	if (!data.pids)
// 	{
// 		ft_dprintf(2, "pipex: PID error: %s\n", strerror(errno));
// 		cleanup_n_exit(ERROR, &data);
// 	}
// 	data.pipe = malloc(sizeof(*data.pipe) * 2 * (hashsize() - 1));
// 	if (!data.pipe)
// 	{
// 		ft_dprintf(2, "pipex: Pipe error: %s\n", strerror(errno));
// 		cleanup_n_exit(ERROR, &data);
// 	}
// 	create_pipes(&data);
// 	return (data);
// }

// /**
//  * Redirects the standard input and output of a process.
//  *
//  * This function is responsible for redirecting the standard input (STDIN) and
//  * standard output (STDOUT) of a process to specified file descriptors. It uses
//  * the `dup2` system call to duplicate the given file descriptors to
//  * STDIN_FILENO and STDOUT_FILENO. If either of the input or output file
//  * descriptors is invalid (less than 0), the function will call
//  * `cleanup_n_exit` to handle the situation. Similarly, if an error occurs
//  * during the redirection process (e.g., `dup2` fails), the function will call
//  * `exit_error` with an appropriate error message or code.
//  *
//  * @param[in] input The file descriptor to be set as the new standard input
//  *                  (STDIN_FILENO).
//  * @param[in] output The file descriptor to be set as the new standard output
//  *                   (STDOUT_FILENO).
//  * @param[in,out] data Pointer to a t_data structure for error handling and
//  *                     other purposes.
//  */
// static void	redirect_io(int input, int output, t_data *data)
//
//{
// 	if (input < 0 || output < 0)
// 		cleanup_n_exit(ERROR, data);
// 	if (dup2(input, STDIN_FILENO) == -1)
// 		cleanup_n_exit(ERROR, data);
// 	if (dup2(output, STDOUT_FILENO) == -1)
// 		cleanup_n_exit(ERROR, data);
// }

// /**
//  * Executes a child process in the Pipex pipeline.
//  *
//  * This function is responsible for configuring and executing a single command
//  * within the pipeline. It redirects the standard input and output for the
//  * child process based on its position in the pipeline:
//  * - The first child uses the initial input file descriptor and the write-end
//  *   of the first pipe.
//  * - The last child uses the read-end of the last pipe and the final output
//  *   file descriptor.
//  * - Middle children use the read-end of their respective input pipe and the
//  *   write-end of their respective output pipe. After setting up the
//  *   redirection, the function closes any file descriptors that are no longer
//  *   needed. It validates the command and its options and then executes the
//  *   command using `execve`. If `execve` fails, it handles the error
//  *   appropriately.
//  *
//  * @param[in,out] data Pointer to the `t_data` structure containing necessary
//  *                     information for the child process, including file
//  *                     descriptors, command options, and environment variables.
//  */
// static void	execute_child_process(t_data *data)
//
//{
// 	if (data->child == 0)
// 		redirect_io(data->input_fd, data->pipe[1], data);
// 	else if (data->child == data->cmd_count - 1)
// 		redirect_io(data->pipe[2 * data->child - 2], data->output_fd, data);
// 	else
// 		redirect_io(data->pipe[2 * data->child - 2],
// 			data->pipe[2 * data->child + 1], data);
// 	close_fds(data);
// 	if (data->cmd_options == NULL || data->cmd_path == NULL)
// 		cleanup_n_exit(ERROR, data);
// 	if (execve(data->cmd_path, data->cmd_options, data->envp) == -1)
// 	{
// 		//cleanup_n_exit(ERROR, data);
// 		ft_printf("pipex: %s: %s\n", data->cmd_options[0], strerror(errno));
// 	}
// }

// /**
//  * Manages the parent process in a pipeline of command executions.
//  *
//  * This function is responsible for closing unnecessary file descriptors and
//  * waiting for child processes to complete their execution. It iterates through
//  * the child processes, using `waitpid` to wait for each child to finish. The
//  * function captures the exit status of the last child process in the pipeline,
//  * which it returns as the overall exit code. It also performs cleanup by
//  * freeing memory allocated for pipes and process IDs.
//  *
//  * @param[in,out] data Pointer to a t_data structure containing necessary
//  *                     information, such as the number of commands, child
//  *                     process IDs, and pipe file descriptors.
//  *
//  * @return The exit status code of the last child process in the pipeline. If no
//  *         exit status is captured, the function defaults to returning 1.
//  */
// static int	execute_parent_process(t_data *data)
//
//{
// 	pid_t	wpid;
// 	int		status;
// 	int		exit_code;

// 	close_fds(data);
// 	data->child--;
// 	exit_code = 1;
// 	while (data->child >= 0)
// 	{
// 		wpid = waitpid(data->pids[data->child], &status, 0);
// 		if (wpid == data->pids[data->cmd_count - 1])
// 		{
// 			if ((data->child == (data->cmd_count - 1)) && WIFEXITED(status))
// 				exit_code = WEXITSTATUS(status);
// 		}
// 		data->child--;
// 	}
// 	free(data->pipe);
// 	free(data->pids);
// 	return (exit_code);
// }

// /**
//  * Manages the execution of a pipeline of commands.
//  *
//  * This function sets up a pipe and forks child processes for each command in the
//  * pipeline. Each child process is responsible for executing one command. The
//  * parent process waits for all child processes to complete and then collects
//  * their exit codes. It handles the creation of pipes, parsing and execution of
//  * commands, and manages potential errors in these processes. If a heredoc is
//  * used, the function also ensures the removal of the temporary file after
//  * execution.
//  *
//  * @param[in,out] data Pointer to a t_data structure containing necessary
//  *                     information for pipelined command execution.
//  *
//  * @return The exit code of the last child process in the pipeline.
//  */
// static int	pipex(t_global *d)
//
//{
// 	int		exit_code;
// 	char	*command;

// 	if (pipe(d->pipe) == -1)
// 	{
// 		ft_dprintf("minishell: %s\n", strerror(errno));
// 		return (1);//ou outro erro
// 	}
// 	d->child = 0;
// 	while (d->child < d->cmd_count)
// 	{
// 		d->cmd_path = get_cmd(d->cmd_options[0], d);
// 		d->pids[d->child] = fork();
// 		if (d->pids[d->child] == -1)
// 			cleanup_n_exit(ft_printf("fork error: %s\n", strerror(errno)), d);
// 		else if (d->pids[d->child] == 0)
// 			execute_child_process(d);
// 		free(d->cmd_path);
// 		d->cmd_path = NULL;
// 		free_array(d->cmd_options);
// 		d->child++;
// 	}
// 	exit_code = execute_parent_process(d);
// 	if (d->heredoc_flag == 1)
// 		unlink(".heredoc.tmp");
// 	return (exit_code);
// }

int	pipecount(t_global *data)
{
	int	result;

	result = 0;
	while (data->hashtable[result])
	{
		result++;
	}
	return (result - 1);
}

void	exec_command(t_global *data, int idx)
{
	char	**args;
	char	*cmd;

	//parse_redirections(data->hashtable[idx]);
	// if (data->hashtable[idx]->content == NULL)
	// 	exit(EXIT_SUCCESS);
	args = hash_to_args(data->hashtable[idx]);
	if (is_builtin(args[0]))
		data->ret = exec_builtin(args, hashsize(data->hashtable[idx]), data);
	else
	{
		data->ret = 127; //arrumar valor de retorno para comandos que nao existem
		cmd = get_cmd(args[0], data);
		if (cmd)
			data->ret = execve(cmd, args, data->env); //consolidar env talvez
		//perror("minishell: execve");
	}
	ft_strarr_free(args, ft_strarr_len(args));
	exit(EXIT_FAILURE);
}

void	create_pipes(int pipes[][2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe:");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	close_pipes(int pipes[][2], int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

// void	setup_dup2(int pipes[][2], int i, int n)
// {
// 	if (i > 0)
// 	{
// 		dup2(pipes[i-1][0], STDIN_FILENO);
// 	}
// 	if (i < n)
// 	{
// 		dup2(pipes[i][1], STDOUT_FILENO);
// 	}
// }

// void	handle_child_process(t_global *data, int pipes[][2], int i, int n)
// {
// 	if (i > 0)
// 		dup2(pipes[i - 1][0], STDIN_FILENO);
// 	if (i < n)
// 		dup2(pipes[i][1], STDOUT_FILENO);
// 	close_pipes(pipes, n);
// 	exec_command(data, i);
// }

void	fork_processes(t_global *data, int pipes[][2], int n)
{
	int	i;

	i = 0;
	while (i <= n)
	{
		data->pid = fork();
		if (data->pid == -1)
		{
			perror("minishell: fork");
			exit(EXIT_FAILURE);
		}
		else if (data->pid == 0)
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < n)
				dup2(pipes[i][1], STDOUT_FILENO);
			close_pipes(pipes, n);
			if (parse_redirections(data->hashtable[i]) == 1)
				return;
			if (data->hashtable[i]->content == NULL)
				return ;
			exec_command(data, i);
		}
		i++;
	}
}

// void	wait_for_children()
// {
// 	while (wait(NULL) > 0);
// }

// int	exec(t_global *data)
// {
// 	int	n;

// 	n = pipecount(data);
// 	int	pipes[n][2];

// 	create_pipes(pipes, n);
// 	fork_processes(data, pipes, n);
// 	close_pipes(pipes, n);
// 	wait_for_children();
// 	return (0);
// }

int	exec(t_global *data)
{
	int	n;
	int	(*pipes)[2];

	n = pipecount(data);
	pipes = malloc(n * sizeof(*pipes));
	if (pipes == NULL)
	{
		perror("minishell: pipes: malloc failed");
		return (-1);
	}
	create_pipes(pipes, n);
	fork_processes(data, pipes, n);
	close_pipes(pipes, n);
	while (wait(NULL) > 0)

	free(pipes);
	return (0);
}

int	prepare_exec(t_global *data)
{
	int		ret;
	char	**args;

	args = hash_to_args(data->hashtable[0]);
	ret = 1;
	if (parse_redirections(data->hashtable[0]) == 1)
		return (1);
	if (pipecount(data) == 0 && args && is_builtin(args[0]))
		exec_builtin(args, hashsize(data->hashtable[0]), data);
	else if (pipecount(data) > -1)
		exec(data);
	ft_strarr_free(args, ft_strarr_len(args));
	// else
	// 	return (exec(data, args));

	return (ret);
}

// // int	exec(t_global *data, char **args)
// //
//{//
// // 	pid_t	pid;
// // 	int		status;

// // 	pid = fork();
// // 	if (pid == 0)
// // 	{
// // 		if (execve(args[0], args, data->env) == -1)
// // 		{
// // 			ft_dprintf(2, "minishell: %s: %s\n", args[0], strerror(errno));
// // 			exit(1);
// // 		}
// // 	}
// // 	else if (pid < 0)
// // 	{
// // 		ft_dprintf(2, "minishell: %s\n", strerror(errno));
// // 		return (1);
// // 	}
// // 	else
// // 	{
// // 		waitpid(pid, &status, 0);
// // 		if (WIFEXITED(status))
// // 			data->ret = WEXITSTATUS(status);
// // 	}
// // 	return (0);
// // }


/*
- check pipe behavior
	- echo hello | tr 'a-z' 'A-Z' segfault
	- does it git input from previous process
- redirects
	- create all files
	- set fds
	- reset fds/dup when needed
*/