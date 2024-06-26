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
	data->ret = 0;
	data->exit = 0;
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
	data->env = ft_strarr_dup(__environ);
	data->is_exec = FALSE;
	data->is_heredoc = FALSE;
	data->exported = NULL;
	data->usr = ft_strdup(ft_getenv("USER", &data));
	init_hashtable(&data->hashtable);
	return (data);
}

void	free_redir_args(char *(*args)[TABLE_SIZE])
{
	int		i;

	i = 0;
	if ((*args)[i])
	{
		while ((*args)[i] != NULL)
		{
			free((*args)[i]);
			i++;
		}
	}
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
			if ((*hashtable)[i]->delimiter)
				free((*hashtable)[i]->delimiter);
			free_redir_args(&(*hashtable)[i]->redir);
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
	close_all_fds();
	ft_memdel((*data)->usr);
	ft_memdel(*data);
	*data = NULL;
}
void	restore_fds(t_global *data)
{
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}
void	clean_input_and_hashtable(t_global **data)
{
	ft_memdel((*data)->usr_input);
	free_hashtable(&(*data)->hashtable);
	restore_fds(*data);
}
