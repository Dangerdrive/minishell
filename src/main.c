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

	//primeiro e segundo e 4º devem ser adicionados. USER deve modificar o valor de user. 
	//123test deve ser invalido (começa com numero)
	//PATH não deve ser alterado, pois não tem igual.
	//XMODIFIERS deve ser modificado para valor vazio

	char *args[] = {"test1=2", "test2=3", "USER=fucker"/* , "test3", "123test", "PATH", "XMODIFIERS=" */, NULL};
	ft_export(args, data);
	
	char *args2[] = {"PATH", "test2", "123", NULL};
	ft_unset(args2, ft_strarr_len(args2), data);
	char *args3[] =	{"test1=exp2", "OLDPWD=qqr coisa", "OUT=", NULL};
	ft_export(args3, data);
	ft_export(NULL, data);
	// ft_env(&data);
	clean_stuff(&data);
	return (0);
}
