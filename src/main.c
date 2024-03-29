#include "../includes/minishell.h"

int	main(void)
{
	static t_global	*data;

	data = init_data();
	data->usr_input = NULL;
	// while (!data->exit)
	// {
	// 	printf("%s ", data->cur_path);
	// 	handle_signal(&data);
	// 	if (readline_and_handle_input(&data) == -1)
	// 		break ;
	// 	ft_memdel(data->usr_input);
	// 	// if (data->exit == true)
	// 	// 	break ;
	// }
	char *args[] = {"test1=2", "test2=3", NULL};
	char *args2[] = {"PATH", NULL};
	ft_export(args, data);
	ft_unset(args2, data);
	ft_export(NULL, data);
	// ft_env(&data);
	clean_stuff(&data);
	return (0);
}
