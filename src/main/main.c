#include "../includes/minishell.h"

static int	print_cwd(t_global **data)
{
	char	*cwd;
	int		home;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_dprintf(2, "\nminishell: %s\n", strerror(errno));
		return (1);
	}
	home = ft_strlen(ft_getenv("HOME", data));
	if (ft_strncmp(cwd, ft_getenv("HOME", data), home) == 0)
		ft_dprintf(1, "%s%s:~%s$ %s",BLUE, (*data)->usr, &cwd[home], END);
	else
		ft_dprintf(1, "%s$ ", cwd);
	free(cwd);
	return (0);
}

int	main(void)
{
	static t_global	*data;
	int				result;

	data = init_data();
	data->usr_input = NULL;
	// data->exit = handle_signal();
	while (!data->exit)
	{
		define_prompt_signals();
		print_cwd(&data);
		if (!data->exit && readline_and_handle_input(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	result = data->ret;
	clean_stuff(&data);
	return (result);
}
