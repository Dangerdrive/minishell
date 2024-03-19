#include "../includes/minishell.h"

int	main(void)
{
	static t_global	*data;

	data = init_data();
	if (copy_env(&data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->usr_input = NULL;
	while (!data->exit)
	{
		printf("%s ", data->cur_path);
		//handle_signal(&data);
		if (readline_and_handle_input(&data) == -1)
			break ;
		ft_memdel(data->usr_input);
		// if (data->exit == true)
		// 	break ;
	}
	if (!data->hashtable[0])
		printf("HELLO111\n");
	else
		printf("%s\n", data->hashtable[0]->content);
	clean_stuff(&data);
	return (0);
}
