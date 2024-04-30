#include "../../includes/minishell.h"

int	ft_echo(char **args, int args_len)
{
	int		i;
	int		n_option;

	i = 1;
	n_option = 0;
	if (args_len > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (args[i])
		{
			ft_dprintf(STDOUT_FILENO, args[i]);
			if (args[i + 1] && args[i][0] != '\0')
				ft_dprintf(STDOUT_FILENO, " ");
			i++;
		}
	}
	if (n_option == 0)
		ft_dprintf(STDOUT_FILENO, "\n");
	return (0);
}
