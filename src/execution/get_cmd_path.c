/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:59:14 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:59:15 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Retrieves the PATH environment variable and splits it into individual paths.
 *
 * @param[in] data Pointer to global data structure.
 * @return Array of paths or NULL if PATH is not set or memory allocation fails.
 */
char	**get_env_paths(t_global *data)
{
	char	*env_path_str;
	char	**paths;
	int		i;
	char	*tmp;

	i = 0;
	env_path_str = ft_getenv("PATH", &data);
	if (!env_path_str)
		return (NULL);
	paths = ft_split(env_path_str, ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(tmp);
		if (!paths[i])
			return (NULL);
		i++;
	}
	return (paths);
}

/**
 * Constructs the full path for a command by appending it to each path in paths.
 *
 * @param[in] cmd Command to find.
 * @param[in] paths Array of directories to search.
 * @return Full path to the executable or NULL if not found or an error occurs.
 */
char	*get_cmd_path(char*cmd, char**paths)
{
	int		i;
	char	*cmd_path;

	cmd_path = NULL;
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (!cmd_path)
		{
			ft_strarr_free(paths, ft_strarr_len(paths));
			ft_dprintf(2, "cmd_path error\n");
			return (NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (NULL);
}

/**
 * Determines the executable path for a command, checking if it's a directory or
 * executable directly or within the PATH directories.
 *
 * @param[in] cmd Command to resolve.
 * @param[in] data Pointer to global data structure.
 * @return Path to the executable or NULL if not found or not executable.
 */
char	*get_cmd(char *cmd, t_global *data)
{
	struct stat	statbuf;
	char		**env_paths;
	char		*cmd_path;

	if (stat(cmd, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			ft_dprintf(2, "minishell: %s: Is a directory\n", cmd);
			external_exit(126);
		}
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		ft_dprintf(2, "minishell: %s: Permission denied\n", cmd);
		external_exit(126);
	}
	env_paths = get_env_paths(data);
	if (!env_paths)
		return (NULL);
	cmd_path = get_cmd_path(cmd, env_paths);
	ft_strarr_free(env_paths, ft_strarr_len(env_paths));
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}
