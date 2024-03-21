#include "../includes/minishell.h"

int	g_signal;

t_global	*init_data(void)
{
	t_global	*data;

	data = ft_calloc(1, sizeof(t_global));
	if (!data)
		return (NULL);
	data->usr_input = NULL;
	data->env = __environ;
	data->cur_path = getcwd(NULL, 0);
	data->exit = 0;
	init_hashtable(&data->hashtable);
	return (data);
}

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = 0;
		printf("g_signal = %d\n", g_signal);
	}
	return ;
}

int	handle_signal(void)
{
	struct sigaction	act;

	g_signal = 1;
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGINT, &act, NULL) == 0)
	{
		printf("G_signal = %d\n", g_signal);
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
