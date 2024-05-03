#include "../../includes/minishell.h"

void	process_heredocs(t_global *data)
{
	int	i;
	int	j;

	j = 0;
	while (data->hashtable[j])
	{
		i = 0;
		while (data->hashtable[j]->redir[i])
		{
			if (ft_strncmp(data->hashtable[j]->redir[i], "<<", 2) == 0)
				write_in_heredoc(data, i, j, &data->hashtable[j]->redir[i][2]);
			i++;
		}
		j++;
	}
}

/**
 * Saves the original file descriptors for STDIN and STDOUT.
 * 
 * @param[in, out] original_fds Array to store the file descriptors.
 */
void	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
}
