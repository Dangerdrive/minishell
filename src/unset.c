#include "minishell.h"

static void	remove_variable(char *arg, t_global *data)
{
	int		position;
	char	**temp;

	if (arg != NULL)
	{
		position = ft_strarr_str(data->env, arg);
		if (position != -1)
		{
			temp = ft_strarr_strrm(data->env, arg);
			data->env = temp;
		}
		position = ft_strarr_str(data->exported, arg);
		if (position != -1)
		{
			temp = ft_strarr_strrm(data->exported, arg);
			data->exported = temp;
		}
	}
}

int	ft_unset(char **args, int args_len, t_global *data)
{
	int		i;

	i = 0;
	if (args_len == 0)
	{
		ft_printf("unset: not enough arguments\n");
		return (1);
	}
	while (i < args_len)
	{
		if (validate_identifier(args[i]) == 0)
			ft_printf("unset: `%s': not a valid identifier\n", args[i]);
		else if (validate_identifier(args[i]) == 1)
			remove_variable(args[i], data);
		i++;
	}
	return (0);
}
