#include "../includes/minishell.h"

int	main(void)
{
	static t_global	*data;

	data = init_data();
	data->usr_input = NULL;
	//data->exit = handle_signal();
	while (!data->exit)
	{
		printf("%s ", data->cur_path);
		if (!data->exit && readline_and_handle_input(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	clean_stuff(&data);
	return (0);
}
