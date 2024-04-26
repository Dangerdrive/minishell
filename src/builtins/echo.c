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
			ft_printf(args[i]);
			if (args[i + 1] && args[i][0] != '\0')
				ft_printf(" ");
			i++;
		}
	}
	if (n_option == 0)
		ft_printf("\n");
	return (0);
}

//in a case like echo $USER$USER, the result have a space in between
//export test test1=123 && echo $test results in 123. Expansion is being ambiguos.