#include "../../includes/minishell.h"

/**
 * Prints the current working directory to standard output.
 * 
 * @return 0 if successful, 1 if an error occurred while retrieving the directory.
 */
int	ft_pwd(void)
{
	char	*cwd;

	cwd = malloc(4096);
	if (getcwd(cwd, 4096))
	{
		ft_printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		free(cwd);
		return (1);
	}
}
