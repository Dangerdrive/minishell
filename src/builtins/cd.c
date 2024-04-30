/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:24:58 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:24:59 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Replaces an existing environment variable or adds a new one based
 * on the argument.
 *
 * @param[in] arg The environment variable in "key=value" format.
 * @param[in, out] data The global data structure containing
 * environment variables.
 */
static void	replace_or_add_env(char *arg, t_global *data)
{
	char	*key;

	key = NULL;
	if (ft_strchr_i(arg, '=') != -1)
	{
		key = ft_strndup(arg, ft_strchr_i(arg, '='));
		ft_strarr_str_replace(data->env, key, arg);
	}
	else if (ft_strchr_i(arg, '=') == -1)
		key = ft_strdup(arg);
	if (ft_strarr_str(data->env, key) == -1
		&& ft_strarr_str(data->env, key) == -1)
		ft_strarr_stradd(&data->env, arg);
	if (key != NULL)
		free(key);
}

/**
 * Updates the current working directory and optionally the
 *  old working directory.
 *
 * @param[in, out] data The global data structure with environment settings.
 * @param[in] option Determines whether to update "OLDPWD" (1) or "PWD" (0).
 * @return 0 on success, 1 if unable to retrieve or update the working
 *  directory.
 */
static int	update_pwds(t_global *data, int option)
{
	char	cwd[4096];
	char	*pwd;

	if (getcwd(cwd, 4096) == NULL)
		return (1);
	if (option == 1)
		pwd = ft_strjoin("OLDPWD=", cwd);
	else
		pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
		return (1);
	replace_or_add_env(pwd, data);
	ft_memdel(pwd);
	return (0);
}

/**
 * Changes the current directory to the previous directory and updates "OLDPWD".
 *
 * @param[in, out] data The global data structure with environment settings.
 * @return 0 on success, 1 if "OLDPWD" is not set or the directory change fails.
 */
static int	go_oldpwd(t_global *data)
{
	int		result;
	char	*path;
	char	*old_path;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	old_path = ft_strjoin("OLDPWD=", cwd);
	path = NULL;
	path = ft_getenv("OLDPWD", &data);
	if (!path)
	{
		ft_dprintf(STDERR_FILENO, "minishell : cd: OLDPWD not set\n");
		return (0);
	}
	result = chdir(path);
	replace_or_add_env(old_path, data);
	ft_memdel(old_path);
	ft_memdel(cwd);
	return (result);
}

/**
 * Changes the current directory to the home directory defined by "HOME".
 *
 * @param[in, out] data The global data structure with environment settings.
 * @return 0 on success, 1 if "HOME" is not set or the directory change fails.
 */
static int	go_home(t_global *data)
{
	int		result;
	char	*path;

	path = NULL;
	update_pwds(data, 1);
	path = ft_getenv("HOME", &data);
	if (!path)
		ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
	if (!path)
		return (0);
	result = chdir(path);
	return (result);
}

/**
 * Processes the "cd" command with arguments to change the current directory.
 *
 * @param[in] args The array of command arguments.
 * @param[in] args_len The length of the args array.
 * @param[in, out] data The global data structure with environment settings.
 * @return 0 on successful directory change, 1 on failure or invalid input.
 */
int	ft_cd(char **args, int args_len, t_global *data)
{
	int		result;

	if (args_len > 2)
	{
		result = 1;
		ft_dprintf(2, "minishell : cd: too many arguments");
	}
	else if (args_len == 1)
		result = go_home(data);
	else if (ft_strcmp(args[1], "-") == 0)
		result = go_oldpwd(data);
	else
	{
		update_pwds(data, 1);
		result = chdir(args[1]);
		if (result < 0)
			result *= -1;
		if (result != 0)
			ft_dprintf(2, "minishell: cd: %s: %s\n", args[1], strerror(errno));
	}
	update_pwds(data, 0);
	return (result);
}
