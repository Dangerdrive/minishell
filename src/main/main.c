#include "../includes/minishell.h"

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
		if (!data->exit && readline_and_handle_input(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	result = data->ret;
	clean_stuff(&data);
	return (result);
}
