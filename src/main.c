#include "../includes/minishell.h"

int	main(void)
{
	static t_global	*data;

	data = init_data();
	if (copy_env(&data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->usr_input = NULL;
	// while (!data->exit)
	// {
	// 	printf("%s ", data->cur_path);
	// 	handle_signal(&data);
	// 	if (readline_and_handle_input(&data) == -1)
	// 		break ;
	// 	ft_memdel(data->usr_input);
	// }
	clean_stuff(&data);
	return (0);
}
