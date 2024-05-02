#include "../../includes/minishell.h"

/**
 * Initializes an array of redirection arguments to NULL.
 *
 * @param[out] args Pointer to the array of redirection arguments.
 */
void	init_redir_args(char *(*args)[TABLE_SIZE])
{
	int	i;

	i = 0;
	while (i < TABLE_SIZE)
	{
		(*args)[i] = NULL;
		i++;
	}
	return ;
}

/**
 * Checks if the current token is a heredoc delimiter and updates accordingly.
 *
 * @param[in, out] node Pointer to the token node to check and update.
 */
void	check_heredoc(t_tkn **node)
{
	if (strncmp((*node)->content, DOUBLE_LESS_THAN, 2) == 0
		&& (*node)->next && !is_special_token((*node)->next->content))
	{
		(*node)->delimiter = ft_strdup((*node)->next->content);
	}
}

/**
 * Updates a list of redirection file paths based on the
 *  given signal and new argument.
 *
 * @param[out] redir Array of redirection paths to update.
 * @param[in] sig The redirection signal ('<', '>', '<<', '>>').
 * @param[in] new_arg The new file path or delimiter to add.
 */
void	update_redir_files_list(char *(*redir)[TABLE_SIZE],
			char *sig, char *new_arg)
{
	int		i;
	char	*new_sig;

	if (is_redir_in(sig) || is_redir_out(sig))
		new_sig = ft_strjoin(sig, " ");
	else
		new_sig = ft_strdup(sig);
	i = 0;
	if ((*redir)[i])
	{
		while ((*redir)[i])
			i++;
	}
	(*redir)[i] = ft_strjoin(new_sig, new_arg);
	free(new_sig);
	return ;
}
