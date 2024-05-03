/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:25 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 23:05:58 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Generates an error message for when getcwd fails.
 *
 * @return A pointer to a string with the error message.
 */
char	*cwd_error(void)
{
	static char	prompt[PATH_MAX];

	ft_bzero(prompt, sizeof(prompt));
	ft_strlcat(prompt, "minishell: ", PATH_MAX);
	ft_strlcat(prompt, strerror(errno), PATH_MAX);
	return (prompt);
}

/**
 * Generates the shell prompt, displaying the current working directory.
 *
 * @param data Pointer to the global data structure.
 * @return A pointer to a string containing the formatted prompt.
 */
char	*print_cwd(t_global **data)
{
	static char	prompt[PATH_MAX];
	char		*cwd;
	int			home;

	cwd = getcwd(NULL, 0);
	ft_bzero(prompt, sizeof(prompt));
	if (!cwd)
		return (cwd_error());
	home = ft_strlen(ft_getenv("HOME", data));
	ft_strlcat(prompt, BLUE, PATH_MAX);
	if (ft_strncmp(cwd, ft_getenv("HOME", data), home) == 0)
	{
		ft_strlcat(prompt, (*data)->usr, PATH_MAX);
		ft_strlcat(prompt, ":~", PATH_MAX);
		ft_strlcat(prompt, &cwd[home], PATH_MAX);
	}
	else
		ft_strlcat(prompt, cwd, PATH_MAX);
	ft_strlcat(prompt, "$ ", PATH_MAX);
	ft_strlcat(prompt, END, PATH_MAX);
	free(cwd);
	return (prompt);
}

/**
 * Handles the prompt input from the user, executing commands as needed.
 *
 * @param data Pointer to the global data structure.
 * @return -1 if an error occurs or the user exits, 1 otherwise.
 */
int	handle_prompt(t_global **data)
{
	int	input;

	input = 0;
	(*data)->usr_input = NULL;
	(*data)->usr_input = readline(print_cwd(data));
	if (!(*data)->usr_input)
		return (-1);
	if ((*data)->usr_input[0])
	{
		add_history((*data)->usr_input);
		input = tokenize(data);
		if (input == -1)
			return (-1);
		if (input == 1)
		{
			input = parse(&(*data)->hashtable, data);
			if (input == 1)
				(*data)->ret = handle_execution(*data);
		}
	}
	return (1);
}
