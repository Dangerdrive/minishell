#include "../../includes/minishell.h"

int		pwd(void)
{
	char	*cwd;

	cwd = getcwd(cwd, 4096);
	if (cwd)
	{
		ft_printf("%s", cwd);
		return (1);
	}
	else
		return (0);
}

int	main(void)
{
	pwd();
	return (0);
}