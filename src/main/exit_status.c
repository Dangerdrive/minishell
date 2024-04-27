#include "../../includes/minishell.h"

int	is_folder(char *command)
{
	struct stat	statbuf;

	if (stat(command, &statbuf) != 0)
		return (0);
	if (S_ISDIR(statbuf.st_mode))
	{
		if (*command == '.')
			command++;
		if (*command == '.')
			command++;
		if (*command == '/')
			return (TRUE);
	}
	return (FALSE);
}

void	external_exit(int exit_status)
{
	if (exit_status == NOT_EXECUTABLE)
		perror("minishell: it is a directory");
	if (exit_status == CMD_NOT_FOUND)
		perror("minishell: command not found");
	rl_clear_history();
	close_all_fds();
	exit(exit_status);
}
