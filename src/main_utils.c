#include "../includes/minishell.h"

int	g_signal;

t_global	*init_data(void)
{
	t_global	*data;

	data = ft_calloc(1, sizeof(t_global));
	if (!data)
		return (NULL);
	data->usr_input = NULL;
	data->env = NULL;
	data->cur_path = getcwd(NULL, 0);
	data->ret = 0;
	data->exit = 0;
	data->env = ft_strarr_dup(__environ);
	data->exported = NULL;
	data->usr = ft_strdup(ft_getenv("USER", &data));
	init_hashtable(&data->hashtable);
	//printf("PATH %s\n", data->cur_path);
	return (data);
}

// void	sig_handler(int signo)
// {
// 	if (signo == SIGINT)
// 	{
// 		g_signal = 0;
// 		printf("g_signal = %d\n", g_signal);
// 	}
// }

// int	handle_signal(void)
// {
// 	struct sigaction	act;

// 	g_signal = 1;
// 	act.sa_handler = sig_handler;
// 	sigemptyset(&act.sa_mask);
// 	act.sa_flags = 0;
// 	sigaction(SIGINT, &act, NULL);
// 	while (g_signal == 1)
// 	{
// 		usleep(100);
// 	}
// 	if (g_signal == 0)
// 	{
// 		//printf("G2_signal = %d\n", g_signal);
// 		return (1);
// 	}
// 	if (sigaction(SIGINT, &act, NULL) == -1) {
//         perror("sigaction");
//         exit(EXIT_FAILURE);
//     }
// 	return (0);
// }

void	free_hashtable(t_tkn *(*hashtable)[TABLE_SIZE])
{
	int		i;
	t_tkn	*temp;

	i = 0;
	temp = NULL;
	while ((*hashtable)[i] != NULL)
	{
		while ((*hashtable)[i] != NULL)
		{
			temp = (*hashtable)[i]->next;
			//printf("FREE::: %s\n", (*hashtable)[i]->content);
			free((*hashtable)[i]->content);
			free((*hashtable)[i]);
			(*hashtable)[i] = temp;
		}
		i++;
	}
}

void	clean_stuff(t_global **data)
{
	if (data == NULL || *data == NULL)
		return ;
	if ((*data)->cur_path != NULL)
	{
		free((*data)->cur_path);
		(*data)->cur_path = NULL;
	}
	ft_strarr_free((*data)->env, ft_strarr_len((*data)->env));
	if ((*data)->exported != NULL)
		ft_strarr_free((*data)->exported, ft_strarr_len((*data)->exported));
	free_hashtable(&(*data)->hashtable);
	rl_clear_history();
	ft_memdel((*data)->usr_input);
	ft_memdel((*data)->usr);
	ft_memdel(*data);
	*data = NULL;
}

void	clean_input_and_hashtable(t_global **data)
{
	ft_memdel((*data)->usr_input);
	free_hashtable(&(*data)->hashtable);
}
