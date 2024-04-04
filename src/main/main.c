#include "../includes/minishell.h"

int	main(void)
{
	static t_global	*data;

	data = init_data();
	data->usr_input = NULL;
	handle_signals(&data);
	while (!data->exit)
	{
		//data->exit = handle_signal();
		//printf("data.exit = %d\n", data->exit);
		printf("%s ", data->cur_path);
		if (data->exit || readline_and_handle_input(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	clean_stuff(&data);
	return (0);
}
