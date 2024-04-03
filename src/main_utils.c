#include "../includes/minishell.h"

int	g_signal = 0;

t_global	*init_data(void)
{
	t_global	*data;

	data = ft_calloc(1, sizeof(t_global));
	if (!data)
		return (NULL);
	data->usr_input = NULL;
	data->env = NULL;
	data->cur_path = getcwd(NULL, 0);
	data->exit = 0;
	data->env = ft_strarr_dup(__environ);
	data->exported = NULL;
	init_hashtable(&data->hashtable);
	printf("PATH %s\n", data->cur_path);
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

void interrupt_handler(int signal) {
	(void)signal;
    printf("SIGINT received, stopping program...\n");
	g_signal = 1;
    // Add any cleanup or exit actions here
}

int	handle_signal(t_global **data)
{
	struct sigaction sa;

    sa.sa_handler = interrupt_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
	{
        perror("sigaction");
        return (EXIT_FAILURE);
    }
	if (g_signal == 1)
	{
		(*data)->exit = 1;
		(*data)->prev_process = 130;
		return (1);
	}
    return (0);
}

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
	free_env(data);
	free_hashtable(&(*data)->hashtable);
	rl_clear_history();
	ft_memdel((*data)->usr_input);
	ft_memdel(*data);
	*data = NULL;
}

void	clean_input_and_hashtable(t_global **data)
{
	ft_memdel((*data)->usr_input);
	free_hashtable(&(*data)->hashtable);
}
