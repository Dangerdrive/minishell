#include "minishell.h"

int	ft_unset(char **args, t_global *data)
{
	int i;
	char **temp;
		
	if (!args)
	{
		ft_printf("unset: not enough arguments\n");
		return (1);
	}
	i = 0;
	while (args[i])
	{
		if (ft_strarr_str(data->env, args[i]))
		{
			temp = ft_strarr_strrm(&data->env, args[i]);
			ft_strarr_free(data->env, ft_strarr_len(data->env));
			data->env = temp;
		}
		i++;
	}
    return (0);
}
