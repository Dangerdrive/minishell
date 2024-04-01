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
	char *args[] = {"test1=2", "test2=3", "USER=fucker", "test3", "123test", "PATH", "XMODIFIERS=", NULL};
	//primeiro e segundo e 4º devem ser adicionados. USER deve modificar o valor de user. 
	//123test deve ser invalido (começa com numero)
	//PATH não deve ser alterado, pois não tem igual.
	//XMODIFIERS deve ser modificado para valor vazio
	char *args2[] = {"PATH", "test2", NULL};
	ft_export(args, data);
	ft_unset(args2, data);
	ft_export(NULL, data);
	// ft_env(&data);
	clean_stuff(&data);
	return (0);
}
