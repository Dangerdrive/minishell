#include "../includes/minishell.h"

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
	data->is_exec = 0;
	data->exported = NULL;
	init_hashtable(&data->hashtable);
	//printf("PATH %s\n", data->cur_path);
	return (data);
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
			if ((*hashtable)[i]->content)
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
