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
		printf("%s ", data->cur_path);
		if (!data->exit && readline_and_handle_input(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	result = data->ret;

	//primeiro e segundo e 4º devem ser adicionados. 
	//USER deve modificar o valor de USER mas não deve modificar o valor de USER_ZDOTDIR
	//123test deve ser invalido (começa com numero)
	//PATH não deve ser alterado, pois não tem igual.
	//XMODIFIERS deve ser modificado para valor vazio
	char *args0[] = {"CC=sucker", NULL};
	ft_export(args0, data);
	char *args[] = {"test1=2", "test2=3", "USER=fucker" , "test3", "123test", "PATH", "XMODIFIERS=", NULL};
	ft_export(args, data);
	
	char *args2[] = {"TERM", "test2", "123", NULL};
	ft_unset(args2, ft_strarr_len(args2), data);
	char *args3[] =	{"test1=exp2", "OLDPWD=qqr coisa", "OUT=", NULL};
	ft_export(args3, data);
	// ft_export(NULL, data);

	char *args_env[] = {"env", "src/", NULL};
	ft_env(args_env, 2, &data);
	clean_stuff(&data);
	return (result);
}
