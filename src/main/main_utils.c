#include "../includes/minishell.h"

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
void	restore_fds_close(t_global *data)
{
	dup2(data->original_stdin, STDIN_FILENO);
	close(data->original_stdin);
	dup2(data->original_stdout, STDOUT_FILENO);
	close(data->original_stdout);
}

void	clean_input_and_hashtable(t_global **data)
{
	int		i;
	char	temp[30];
	char	*num_part;

	ft_memdel((*data)->usr_input);
	free_hashtable(&(*data)->hashtable);
	restore_fds_close(*data);
	restore_fds((*data)->original_fds);
	(*data)->original_fds[1] = STDOUT_FILENO;
	printf("ori_fds0: %d | ori_fds1: %d\n", (*data)->original_fds[0], (*data)->original_fds[1]);
	i = 0;
	while (i < 100)
	{
		strcpy(temp, ".tmp/heredoc");
		num_part = ft_itoa(i);
		if (num_part)
		{
			strcat(temp, num_part);
			free(num_part);
		}
		unlink(temp);
		i++;
	}
}
