#include "../../includes/minishell.h"

/**
 * Prints the current working directory.
 *
 * Allocates a 4096-byte buffer for the directory path, using getcwd to retrieve
 * and ft_printf to print it. The buffer size assumes a typical Linux system
 * limit. It returns 1 on success or 0 on failure, ensuring to free the allocated
 * buffer in both cases to prevent memory leaks.
 *
 * Return values:
 * - 1: Success, directory printed.
 * - 0: Failure, getcwd error.
 */
int		pwd(void)
{
	char	*cwd;

	cwd = malloc(4096);
	if (getcwd(cwd, 4096))
	{
		ft_printf("%s\n", cwd);
		free(cwd);
		return (1);
	}
	else
	{
		free(cwd);
		return (0);
	}
}
