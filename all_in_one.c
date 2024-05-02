// Combined C source files
// File: src/builtins/cd.c
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

// File: src/builtins/echo.c
#include "../../includes/minishell.h"

/**
 * Implements the echo command with an option to suppress trailing newline.
 * 
 * @param[in] args The arguments passed to the echo command.
 * @param[in] args_len The number of arguments.
 * @return Always returns 0 to indicate successful execution.
 */
int	ft_echo(char **args, int args_len)
{
	int		i;
	int		n_option;

	i = 1;
	n_option = 0;
	if (args_len > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (args[i])
		{
			ft_dprintf(STDOUT_FILENO, args[i]);
			if (args[i + 1] && args[i][0] != '\0')
				ft_dprintf(STDOUT_FILENO, " ");
			i++;
		}
	}
	if (n_option == 0)
		ft_dprintf(STDOUT_FILENO, "\n");
	return (0);
}

// File: src/builtins/env.c
#include "../includes/minishell.h"

/**
 * Retrieves the value of an environment variable from either the 'env' or
 * 'exported' arrays in the global data structure.
 * 
 * @param[in] name The name of the environment variable to find.
 * @param[in] data Pointer to the global data structure.
 * @return The value of the environment variable if found, NULL otherwise.
 */
char	*ft_getenv(char *name, t_global **data)
{
	int		i;
	char	*value;

	i = 0;
	while ((*data)->env[i])
	{
		if (ft_strncmp((*data)->env[i], name, ft_strlen(name)) == 0)
		{
			value = ft_strchr((*data)->env[i], '=') + 1;
			return (value);
		}
		i++;
	}
	i = 0;
	while ((*data)->exported && (*data)->exported[i])
	{
		if (ft_strncmp((*data)->exported[i], name, ft_strlen(name)) == 0)
		{
			value = ft_strchr((*data)->exported[i], '=') + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}

/**
 * Prints all environment variables from the 'env' and 'exported' arrays
 * in the global data structure.
 * 
 * @param[in] data Pointer to the global data structure.
 * @return 0 on success, 1 if the environment array is empty.
 */
static int	env_print(t_global **data)
{
	int		i;
	t_bool	exp_ended;

	exp_ended = FALSE;
	if (!(*data)->env)
		return (1);
	i = 0;
	while ((*data)->env[i])
	{
		printf("%s\n", (*data)->env[i]);
		if (!exp_ended && (*data)->exported && (*data)->exported[i])
		{
			if (ft_strchr_i((*data)->exported[i], '=') != -1)
				printf("%s\n", (*data)->exported[i]);
			if (!(*data)->exported[i + 1])
				exp_ended = TRUE;
		}
		i++;
	}
	while (!exp_ended && (*data)->exported && (*data)->exported[i++ - 1])
		if (ft_strchr_i((*data)->exported[i], '=') != -1)
			printf("%s\n", (*data)->exported[i]);
	return (0);
}

/**
 * Executes the 'env' command to print environment variables, handling
 * errors and permissions for additional arguments.
 * 
 * @param[in] args The arguments passed to the env command.
 * @param[in] args_len The number of arguments.
 * @param[in, out] data Pointer to the global data structure.
 * @return 0 on success, 1 on error or if arguments are invalid.
 */
int	ft_env(char **args, int args_len, t_global **data)
{
	if (args_len == 1)
		return (env_print(data));
	else if (args_len > 1)
	{
		if (access(args[1], F_OK) == 0)
		{
			ft_dprintf(2, "env: %s: Permission denied\n", args[1]);
			(*data)->ret = PERMISSION_DENIED;
		}
		else
		{
			ft_dprintf(2, "env: %s: No such file or directory\n", args[1]);
			(*data)->ret = 1;
		}
		return (1);
	}
	return (0);
}

// File: src/builtins/exec_builtins.c
#include "../../includes/minishell.h"

/**
 * Determines if a given command string matches any known shell builtin commands.
 * 
 * @param[in] command The command string to check.
 * @return 1 (TRUE) if the command is a builtin, otherwise 0 (FALSE).
 */
t_bool	is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

/**
 * Executes a shell builtin command based on the parsed arguments.
 * Manages redirection and restores original file descriptors.
 * 
 * @param[in] args Array of arguments including the command.
 * @param[in] args_len Number of arguments passed.
 * @param[in, out] data Global data context including file descriptors.
 * @return 0 on success or 1 on failure to execute the command or manage 
 * redirections.
 */
int	exec_builtin(char **args, int args_len, t_global *data)
{
	int		result;

	result = 0;
	if (handle_redirects(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (1);
	}
	if (ft_strcmp(args[0], "echo") == 0)
		result = ft_echo(args, args_len);
	if (ft_strcmp(args[0], "cd") == 0)
		result = ft_cd(args, args_len, data);
	if (ft_strcmp(args[0], "pwd") == 0)
		result = ft_pwd();
	if (ft_strcmp(args[0], "env") == 0)
		result = ft_env(args, args_len, &data);
	if (ft_strcmp(args[0], "export") == 0)
		result = ft_export(args, args_len, data);
	if (ft_strcmp(args[0], "unset") == 0)
		result = ft_unset(args, args_len, data);
	if (ft_strcmp(args[0], "exit") == 0)
		result = ft_exit(args, ft_strarr_len(args), data);
	restore_original_fds(data->original_fds);
	return (result);
}

// File: src/builtins/exit.c
#include "../../includes/minishell.h"

/**
 * Checks if the given command is a shell builtin command.
 * 
 * @param[in] command The command to check.
 * @return 1 if the command is a builtin, 0 otherwise.
 */
static t_bool	argument_is_valid(char *arg)
{
	int	i;
	int	len_limit;

	len_limit = 19;
	i = 0;
	if (arg == NULL)
		return (0);
	if (arg[0] == '-' || arg[0] == '+')
	{
		i++;
		len_limit++;
	}
	if (arg[i] == '9' && arg[i + 1] > '2')
		len_limit--;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (FALSE);
		i++;
	}
	if (i > len_limit)
		return (0);
	return (TRUE);
}

/**
 * Executes a builtin command, handling redirections and restoring file
 * descriptors as necessary.
 * 
 * @param[in] args The arguments of the command.
 * @param[in] args_len The number of arguments.
 * @param[in, out] data Global data structure containing redirection info.
 * @return Result of the command execution or 1 if redirection fails.
 */
int	ft_exit(char **args, int args_len, t_global *data)
{
	if (args_len == 1)
	{
		if (pipecount(data) == 0)
			ft_printf("exit\n");
		data->ret = 0;
		data->exit = TRUE;
	}
	else if (args_len > 1 && !argument_is_valid(args[1]))
	{
		data->ret = 2;
		ft_dprintf(2, "exit: %s: numeric argument required\n", args[1]);
		data->exit = TRUE;
	}
	else if (args_len > 2 && argument_is_valid(args[1]))
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		data->ret = 1;
	}
	else if (args_len == 2 && argument_is_valid(args[1]))
	{
		ft_printf("exit\n");
		data->ret = ft_atoi(args[1]);
		data->exit = TRUE;
	}
	return (data->ret);
}

// File: src/builtins/export.c
#include "../includes/minishell.h"

void	ft_strarr_str_exp_rplc(char **strarr, char *target, char *replcmt);
int		ft_strarr_str_exp(char **strarr, const char *target);

/**
 * Prints each environment variable in the 'sorted_arr' array in a format
 * suitable for export command output.
 * 
 * @param[in] sorted_arr Sorted array of environment variables.
 */
void	print_exp(char **sorted_arr)
{
	char	*key;
	char	*value;
	int		i;
	int		delim_pos;

	i = 0;
	while (sorted_arr && sorted_arr[i])
	{
		delim_pos = ft_strchr_i(sorted_arr[i], '=');
		if (delim_pos != -1)
		{
			key = strndup(sorted_arr[i], delim_pos);
			value = ft_strdup(sorted_arr[i] + delim_pos + 1);
			if (value)
				ft_printf("declare -x %s=\"%s\"\n", key, value);
			else
				ft_printf("declare -x %s=\"\"\n", key);
			free (key);
			free (value);
		}
		else
			ft_printf("declare -x %s\n", sorted_arr[i]);
		i++;
	}
}

/**
 * Handles the 'export' command when no arguments are provided, printing
 * all environment variables.
 * 
 * @param[in] data Pointer to the global data structure.
 */
void	export_no_args(t_global *data)
{
	char	**sorted_env;
	char	**sorted_export;

	sorted_env = ft_strarr_sort(data->env);
	sorted_export = ft_strarr_sort(data->exported);
	print_exp(sorted_env);
	print_exp(sorted_export);
	ft_strarr_free(sorted_env, ft_strarr_len(sorted_env));
	ft_strarr_free(sorted_export, ft_strarr_len(sorted_export));
}

/**
 * Validates if the provided identifier is a valid shell variable name.
 * 
 * @param[in] str String to validate as a shell variable name.
 * @return TRUE if the identifier is valid, FALSE otherwise.
 */
t_bool	identifier_is_valid(char *str)
{
	int		i;
	char	*key;

	if (str == NULL)
		return (FALSE);
	key = str;
	if (ft_strchr_i(str, '=') != -1)
		key = ft_strndup(str, ft_strchr_i(str, '='));
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (FALSE);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_' && key[i] != '=')
			return (FALSE);
		i++;
	}
	if (ft_strchr_i(str, '=') != -1)
		free(key);
	return (TRUE);
}

/**
 * Adds or replaces an environment or exported variable with the given argument.
 * 
 * @param[in] arg The variable in the form 'key=value' or 'key'.
 * @param[in, out] data Pointer to the global data structure.
 */
void	replace_or_add(char *arg, t_global *data)
{
	char	*key;

	key = NULL;
	if (ft_strchr_i(arg, '=') != -1)
	{
		key = ft_strndup(arg, ft_strchr_i(arg, '='));
		ft_strarr_str_exp_rplc(data->env, key, arg);
		ft_strarr_str_exp_rplc(data->exported, key, arg);
	}
	else if (ft_strchr_i(arg, '=') == -1)
		key = ft_strdup(arg);
	if (ft_strarr_str_exp(data->env, key) == -1
		&& ft_strarr_str_exp(data->exported, key) == -1
		&& ft_strarr_str_exp(__environ, key) == -1)
		ft_strarr_stradd(&data->exported, arg);
	else if (ft_strarr_str_exp(data->env, key) == -1
		&& ft_strarr_str_exp(data->exported, key) == -1)
		ft_strarr_stradd(&data->env, arg);
	if (key != NULL)
		free(key);
}

/**
 * Executes the 'export' command, adding or updating variables in the environment
 * and handling validation and sorting.
 * 
 * @param[in] args Array of arguments for the command.
 * @param[in] args_len Number of arguments provided.
 * @param[in, out] data Pointer to the global data structure.
 * @return 0 if successful, 1 if an error occurred.
 */
int	ft_export(char **args, int args_len, t_global *data)
{
	int		i;
	int		ret;

	ret = 0;
	if (args_len <= 1)
		export_no_args(data);
	else if (args_len > 1)
	{
		i = 1;
		while (i < args_len)
		{
			if (!identifier_is_valid(args[i]))
			{
				ft_dprintf(2, "export: `%s': not a valid identifier\n", args[i]);
				ret = 1;
			}
			else if (identifier_is_valid(args[i]))
				replace_or_add(args[i], data);
		i++;
		}
	}
	return (ret);
}

// File: src/builtins/export_utils.c
#include "../includes/minishell.h"

/**
 * Replaces an element in a string array matching the target with a replacement.
 * 
 * @param[in, out] strarr Array of strings containing key=value pairs.
 * @param[in] target The key to match for replacement.
 * @param[in] replcmt The string to replace the matching element.
 */
void	ft_strarr_str_exp_rplc(char **strarr, char *target, char *replcmt)
{
	int		i;
	int		len;
	char	*strarr_key;

	if (!strarr || !target || !replcmt)
		return ;
	len = ft_strarr_len(strarr);
	i = 0;
	while (i < len)
	{
		if (ft_strchr_i(strarr[i], '=') != -1)
			strarr_key = ft_strndup(strarr[i], ft_strchr_i(strarr[i], '='));
		else
			strarr_key = ft_strdup(strarr[i]);
		if (strarr[i] && ft_strcmp(strarr_key, target) == 0)
		{
			free(strarr[i]);
			strarr[i] = ft_strdup(replcmt);
			if (!strarr[i])
				return ;
		}
		free(strarr_key);
		i++;
	}
}

/**
 * Searches for a string key in a string array and returns its index.
 * 
 * @param[in] strarr Array of strings containing key=value pairs.
 * @param[in] target The key to search for.
 * @return Index of the matching key, or -1 if not found.
 */
int	ft_strarr_str_exp(char **strarr, const char *target)
{
	int		i;
	char	*strarr_key;

	if (!strarr || !target)
		return (-1);
	i = 0;
	while (strarr[i])
	{
		if (ft_strchr_i(strarr[i], '=') != -1)
			strarr_key = ft_strndup(strarr[i], ft_strchr_i(strarr[i], '='));
		else
			strarr_key = ft_strdup(strarr[i]);
		if (ft_strcmp(strarr_key, target) == 0)
			return (i);
		free(strarr_key);
		i++;
	}
	return (-1);
}

// File: src/builtins/pwd.c
#include "../../includes/minishell.h"

/**
 * Prints the current working directory to standard output.
 * 
 * @return 0 if successful, 1 if an error occurred while retrieving the 
 * directory.
 */
int	ft_pwd(void)
{
	char	*cwd;

	cwd = malloc(4096);
	if (getcwd(cwd, 4096))
	{
		ft_printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		free(cwd);
		return (1);
	}
}

// File: src/builtins/unset.c
#include "../../includes/minishell.h"

/**
 * Removes strings matching a target from an array of strings.
 * 
 * @param[in] strarr Array of strings to filter.
 * @param[in] tgt Target string to match for removal.
 * @return New array without the matching strings or NULL on failure.
 */
char	**ft_strarr_strrm_unset(char **strarr, const char *tgt)
{
	int		len;
	int		i;
	int		j;
	char	**new_arr;

	len = ft_strarr_len(strarr);
	new_arr = (char **)malloc(sizeof(char *) * (len + 1));
	if (!new_arr)
		return (NULL);
	i = 0;
	j = 0;
	while (strarr[i] != NULL)
	{
		if (!key_matches_target(strarr[i], tgt))
		{
			new_arr[j] = strarr[i];
			j++;
		}
		else
			free(strarr[i]);
		i++;
	}
	new_arr[j] = NULL;
	free(strarr);
	return (new_arr);
}

/**
 * Searches for a target string in an array and returns its index.
 * 
 * @param[in] strarr Array of strings.
 * @param[in] target Target string to find.
 * @return Index of the matching string or -1 if not found.
 */
static int	ft_strarr_str_unset(char **strarr, const char *target)
{
	int		i;
	char	*key;

	key = NULL;
	if (!strarr || !target)
		return (-1);
	i = 0;
	while (strarr[i])
	{
		key = strndup(strarr[i], ft_strchr_i(strarr[i], '='));
		if ((ft_strcmp(key, target) == 0))
		{
			free(key);
			return (i);
		}
		free(key);
		i++;
	}
	return (-1);
}

/**
 * Removes a specified environment variable from two arrays.
 * 
 * @param[in] arg Environment variable to remove.
 * @param[in, out] data Global data containing the environment arrays.
 */
static void	remove_variable(char *arg, t_global *data)
{
	int		position;
	char	**temp;

	if (arg != NULL)
	{
		position = ft_strarr_str_unset(data->env, arg);
		if (position != -1)
		{
			temp = ft_strarr_strrm_unset(data->env, arg);
			data->env = temp;
		}
		position = ft_strarr_str_unset(data->exported, arg);
		if (position != -1)
		{
			temp = ft_strarr_strrm_unset(data->exported, arg);
			data->exported = temp;
			if (data->exported[0] == NULL)
			{
				free(data->exported);
				data->exported = NULL;
			}
		}
	}
}

/**
 * Unsets environment variables specified in args.
 * 
 * @param[in] args Array of environment variables to unset.
 * @param[in] args_len Number of elements in args.
 * @param[in, out] data Global data structure.
 * @return 0 on success, 1 on error or invalid arguments.
 */
int	ft_unset(char **args, int args_len, t_global *data)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	if (args_len <= 1)
		return (0);
	while (i < args_len)
	{
		if (!unset_identifier_is_valid(args[i]))
		{
			ft_dprintf(2, "unset: `%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		else if (unset_identifier_is_valid(args[i]))
			remove_variable(args[i], data);
		i++;
	}
	return (ret);
}

// File: src/builtins/unset_utils.c
#include "../../includes/minishell.h"

/**
 * Validates if the provided string is a valid identifier for unset operations.
 * 
 * @param[in] str The string to validate as an identifier.
 * @return TRUE if the identifier is valid (alphanumeric or underscore, not
 *         starting with a digit), FALSE otherwise.
 */
t_bool	unset_identifier_is_valid(char *str)
{
	int		i;

	if (str == NULL)
		return (FALSE);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (FALSE);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/**
 * Checks if the key part of a 'key=value' string matches the specified target.
 * 
 * @param[in] strarr_elem The string element in 'key=value' format.
 * @param[in] tgt The target key to match.
 * @return 1 if the key matches the target, 0 otherwise.
 */
int	key_matches_target(const char *strarr_elem, const char *tgt)
{
	char	*key;
	int		result;

	key = strndup(strarr_elem, ft_strchr_i(strarr_elem, '='));
	result = ft_strcmp(key, tgt);
	free(key);
	return (result == 0);
}

// File: src/execution/exec_one_process.c
#include "../../includes/minishell.h"

void	write_in_heredoc(t_global *data, int heredoc_number,
			int hash_number, char *eof);

/**
 * Executes a non-builtin command found in the system's PATH.
 * 
 * @param[in] args Array of arguments for the command.
 * @param[in, out] data Global data structure including the environment.
 * @return Returns the status set in data->ret, may exit directly on failure.
 */
int	exec_nonbuiltin(char **args, t_global *data)
{
	char	*cmd;

	if (!args[0][0] || !ft_strcmp(*args, " "))
		external_exit(EXIT_SUCCESS);
	cmd = get_cmd(args[0], data);
	if (cmd == NULL)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", args[0]);
		ft_strarr_free(args, ft_strarr_len(args));
		external_exit(CMD_NOT_FOUND);
	}
	if (is_folder(args[0]))
		external_exit(NOT_EXECUTABLE);
	if (cmd)
	{
		if (execve(cmd, args, data->env) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
				args[0], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	return (data->ret);
}

/**
 * Handles the execution of a non-builtin command by waiting for its completion.
 * 
 * @param[in] data Global data structure.
 * @param[in] args Command arguments.
 * @param[in] pid Process ID of the forked process executing the command.
 * @return Exit status of the command or error signals.
 */
int	exec_nonbuiltin_and_wait(t_global *data, char **args, int pid)
{
	int	status;
	int	ret;

	ret = 1;
	status = 0;
	if (pid == -1)
		perror("minishell: fork");
	else if (pid == 0)
	{
		status = exec_nonbuiltin(args, data);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	if (WIFSIGNALED(status))
		ret = handle_signal_interrupt(status, TRUE);
	else if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	if (ret == 1 && status != 0)
		ret = status;
	return (ret);
}

/**
 * Executes a non-builtin command, managing redirection and process forking.
 * 
 * @param[in, out] data Global data structure with redirection information.
 * @param[in] args Command arguments.
 * @return Status code of command execution or fork process.
 */
int	exec_nonbuiltin_onfork(t_global *data, char **args)
{
	int		pid;
	int		ret;
	int		i;

	i = 0;
	ret = 1;
	while (data->hashtable[0]->redir[i])
	{
		if (ft_strncmp(data->hashtable[0]->redir[i], "<<", 2) == 0)
			write_in_heredoc(data, i, 0, &data->hashtable[0]->redir[i][2]);
		i++;
	}
	if (handle_redirects(data, data->original_fds) == 0)
	{
		restore_original_fds(data->original_fds);
		return (ret);
	}
	if (args && args[0] && !is_builtin(args[0]))
	{
		pid = fork();
		define_exec_signals(pid);
		ret = exec_nonbuiltin_and_wait(data, args, pid);
		restore_original_fds(data->original_fds);
	}
	return (ret);
}

/**
 * Manages the execution of a single process, either builtin or non-builtin.
 * 
 * @param[in, out] data Global data structure.
 * @return Status code of the executed process.
 */
int	exec_one_process(t_global *data)
{
	int		ret;
	char	**args;

	ret = 1;
	args = NULL;
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (args && args[0] && is_builtin(args[0]))
		ret = exec_builtin(args, hashsize(data->hashtable[0]), data);
	else
		ret = exec_nonbuiltin_onfork(data, args);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	restore_original_fds(data->original_fds);
	return (ret);
}

// File: src/execution/exec_processes.c
#include "../../includes/minishell.h"

void	restore_fds(int original_fds[2]);
int		wait_for_children(int children_pid[1024]);

/**
 * Saves the original file descriptors for STDIN and STDOUT.
 * 
 * @param[in, out] original_fds Array to store the file descriptors.
 */
static void	save_original_fds(int original_fds[2])
{
	original_fds[0] = dup(STDIN_FILENO);
	original_fds[1] = dup(STDOUT_FILENO);
}

/**
 * Initializes an array to store child process IDs based on the number of pipes.
 * 
 * @param[in] data Global data structure containing command and pipe information.
 * @return Pointer to the array of child PIDs or NULL if memory allocation fails.
 */
static int	*init_children_pid(t_global *data)
{
	int		*children_pid;
	size_t	size;

	size = sizeof(int) * (pipecount(data) + 2);
	children_pid = malloc(size);
	if (!children_pid)
		return (NULL);
	ft_bzero(children_pid, size);
	return (children_pid);
}

/**
 * Executes a builtin command in a forked process and exits with the command 
 * status.
 * 
 * @param[in] args Arguments for the builtin command.
 * @param[in] idx Index of the command in the global hashtable.
 * @param[in, out] data data structure with environment and process info.
 */
int	execute_forked_builtin(char **args, int idx, t_global *data)
{
	int	exit_status;

	exit_status = exec_builtin(args, hashsize(data->hashtable[idx]), data);
	ft_strarr_free(args, ft_strarr_len(args));
	rl_clear_history();
	data->ret = exit_status;
	exit(exit_status);
}

/**
 * Determines if the command is builtin or not and executes accordingly 
 * in a forked process.
 * 
 * @param[in, out] data  data structure with environment and command info.
 * @param[in] idx Index of the command in the global hashtable.
 */
static void	execute_forked_command(t_global *data, int idx)
{
	char	**args;

	close_extra_fds();
	args = hash_to_args(data->hashtable[idx]);
	if (args)
	{
		if (is_builtin(args[0]))
			execute_forked_builtin(args, idx, data);
		else
		{
			exec_nonbuiltin(args, data);
		}
	}
	else
		exit(0);
}

// /**
//  * Executes all processes described in the global hashtable,
//  *  handling forks and pipes.
//  * 
//  * @param[in, out] data Global data structure containing 
//  * commands and redirections.
//  * @return The exit status of the last process executed.
//  */
// int	exec_processes(t_global *data)
// {
// 	int	*children_pid;
// 	int	i;
// 	int j;

// 	i = 0;
// 	j = 0;
// 	save_original_fds(data->original_fds);
// 	while (data->hashtable[j])
// 	{
// 		while (data->hashtable[j]->redir[i])
// 		{
// 			if (ft_strncmp(data->hashtable[0]->redir[i], "<<", 2) == 0)
// 				write_in_heredoc(data, i, j, &data->hashtable[j]->redir[i][2]);
// 			i++;
// 		}
// 		j++;
// 	}
// 	children_pid = init_children_pid(data);
// 	while (data->hashtable[i])
// 	{
// 		handle_pipe(data->original_fds[OUT], data,
// 			data->hashtable[i], data->hashtable);
// 		children_pid[i] = fork();
// 		define_exec_signals(children_pid[i]);
// 		if (children_pid[i] == -1)
// 			ft_dprintf(2, "minishell: %s: %s\n", "fork", strerror(errno));
// 		else if (children_pid[i] == 0)
// 		{
// 			handle_redirects_for_pipes(&data->hashtable[i]->redir);
// 			execute_forked_command(data, i);
// 		}
// 		i++;
// 	}
// 	restore_fds(data->original_fds);
// 	i = wait_for_children(children_pid);
// 	free(children_pid);
// 	return (i);
// }

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

int	handle_child_processes(t_global *data, int *children_pid)
{
	int	i;

	i = 0;
	while (data->hashtable[i])
	{
		handle_pipe(data->original_fds[OUT], data,
			data->hashtable[i], data->hashtable);
		children_pid[i] = fork();
		define_exec_signals(children_pid[i]);
		if (children_pid[i] == -1)
		{
			ft_dprintf(2, "minishell: %s: %s\n", "fork", strerror(errno));
			continue ;
		}
		if (children_pid[i] == 0)
		{
			handle_redirects_for_pipes(&data->hashtable[i]->redir);
			execute_forked_command(data, i);
		}
		i++;
	}
	return (i);
}

int	exec_processes(t_global *data)
{
	int	*children_pid;
	int	i;

	save_original_fds(data->original_fds);
	process_heredocs(data);
	children_pid = init_children_pid(data);
	i = handle_child_processes(data, children_pid);
	restore_fds(data->original_fds);
	i = wait_for_children(children_pid);
	free(children_pid);
	return (i);
}

// File: src/execution/exec_processes_utils.c
#include "../../includes/minishell.h"

/**
 * Checks if the termination signal of a child process is SIGINT (Ctrl-C).
 * 
 * @param[in] status The exit status of the child process.
 * @return 1 if the signal is SIGINT, otherwise 0.
 */
static int	is_control_c(int status)
{
	return (WTERMSIG(status) == SIGINT);
}

/**
 * Checks if the termination signal of a child process is SIGQUIT (Ctrl-\).
 * 
 * @param[in] status The exit status of the child process.
 * @return 1 if the signal is SIGQUIT, otherwise 0.
 */
static int	is_control_slash(int status)
{
	return (WTERMSIG(status) == SIGQUIT);
}

/**
 * Handles the signal interruption if a child process terminates due to a signal.
 * 
 * @param[in] status The exit status of the child process.
 * @param[in] is_last_child Flag to indicate if this is the last child.
 * @return Modified exit status incorporating the signal number.
 */
int	handle_signal_interrupt(int status, int is_last_child)
{
	if (is_control_c(status))
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	if (is_control_slash(status) && is_last_child)
	{
		ft_putstr_fd("Quit\n", STDOUT_FILENO);
	}
	return (INTERRUPT + WTERMSIG(status));
}

/**
 * Waits for a child process to terminate and returns its exit status.
 * 
 * @param[in] child_pid PID of the child process.
 * @param[in] is_last_child Flag to indicate if this is the last child process.
 * @return The exit status of the child, or EXIT_FAILURE on error.
 */
int	wait_for_child(int child_pid, int is_last_child)
{
	int	status;

	if (child_pid == FORK_ERROR)
		return (EXIT_FAILURE);
	if (waitpid(child_pid, &status, 0) == -1)
		ft_dprintf(2, "minishell: waitpid: %s\n", strerror(errno));
	if (WIFSIGNALED(status))
		return (handle_signal_interrupt(status, is_last_child));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

/**
 * Waits for all child processes specified in the array to terminate, handling
 * their respective exit statuses.
 * 
 * @param[in] children_pid Array of PIDs for the child processes.
 * @return The exit status of the last child process.
 */
int	wait_for_children(int children_pid[1024])
{
	int	i;
	int	exit_status;
	int	is_last_child;

	i = 0;
	exit_status = 0;
	while (children_pid[i] != 0)
	{
		is_last_child = children_pid[i + 1] == 0;
		exit_status = wait_for_child(children_pid[i], is_last_child);
		i++;
	}
	close_extra_fds();
	return (exit_status);
}

// File: src/execution/exec_utils.c
#include "../../includes/minishell.h"

/**
 * Calculates the number of tokens in a linked list representing
 *  a command hash table.
 * 
 * @param[in] hashtable Pointer to the head of the token linked list.
 * @return The count of tokens or -1 if the hashtable is NULL.
 */
int	hashsize(t_tkn *hashtable)
{
	int		i;
	t_tkn	*temp;

	if (!hashtable)
		return (-1);
	temp = hashtable;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

/**
 * Checks if the current node should be considered an empty string based on 
 * specific conditions related to surrounding tokens.
 * 
 * @param[in] node Double pointer to the current token node.
 * @return TRUE if conditions are met to treat as an empty string, 
 * FALSE otherwise.
 */
t_bool	check_empty_str(t_tkn **node)
{
	if ((*node)->prev && ((*node)->prev->content
			&& !ft_strcmp((*node)->prev->content, "")
			&& (*node)->prev->space_after == TRUE)
		&& (*node)->prev->prev && (*node)->prev->prev->space_after == TRUE)
		return (TRUE);
	return (FALSE);
}

/**
 * Fills an array of arguments by joining tokens based on their spacing.
 * 
 * @param[in, out] node Pointer to the current node being processed.
 * @param[out] args Array to fill with arguments.
 * @param[in] i Index at which to insert the new argument.
 */
void	fill_args(t_tkn	**node, char **args, int i)
{
	char	*arg_tmp;

	arg_tmp = NULL;
	if (check_empty_str(node))
		args[i] = ft_strjoin(" ", (*node)->content);
	else if (!(*node)->prev
		|| ((*node)->prev && ((*node)->prev->space_after == TRUE)
			&& (*node)->content))
		args[i] = ft_strdup((*node)->content);
	while (((*node)->space_after == FALSE) && (*node)->next)
	{
		if (!ft_strcmp((*node)->next->content, "") && (*node)->next->next
			&& (*node)->next->space_after == FALSE)
		{
			arg_tmp = ft_strjoin(args[i], (*node)->next->next->content);
			*node = (*node)->next;
		}
		else
			arg_tmp = ft_strjoin(args[i], (*node)->next->content);
		free(args[i]);
		args[i] = arg_tmp;
		(*node) = (*node)->next;
	}
}

/**
 * Converts a hash table of token nodes into an array of strings.
 * 
 * @param[in] node Head of the token linked list.
 * @return Array of strings constructed from the hash table.
 */
char	**hash_to_args(t_tkn *node)
{
	char	**args;
	t_tkn	*temp;
	int		i;
	int		args_count;

	if (!node->content && !node->next)
		return (NULL);
	args_count = hashsize(node);
	args = ft_calloc((args_count + 1), sizeof(char *));
	temp = node;
	i = 0;
	while (temp)
	{
		fill_args(&temp, args, i);
		temp = temp->next;
		i++;
	}
	args[args_count] = NULL;
	return (args);
}

/**
 * Restores the original file descriptors for standard input and output.
 * 
 * @param[in] original_fds Array containing the original file descriptors
 *  to restore.
 */
void	restore_fds(int original_fds[2])
{
	redirect_fd(original_fds[IN], STDIN_FILENO);
	redirect_fd(original_fds[OUT], STDOUT_FILENO);
}

// File: src/execution/get_cmd_path.c
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

// File: src/execution/handle_exec.c
#include "../../includes/minishell.h"

/**
 * Handles the execution of commands stored in the global data's hashtable,
 * managing single and multiple processes based on pipe presence.
 *
 * @param[in, out] data Global structure containing execution details.
 * @return Execution result stored in data->ret.
 */
int	handle_execution(t_global *data)
{
	char	**args;

	args = NULL;
	if (data->hashtable[0] && data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	if (pipecount(data) == 0)
		data->ret = exec_one_process(data);
	else if (pipecount(data) > 0)
		data->ret = exec_processes(data);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	return (data->ret);
}

// File: src/execution/pipes.c
#include "../../includes/minishell.h"

/**
 * Counts the number of command nodes in the global hashtable.
 *
 * @param[in] data Pointer to the global data structure.
 * @return Number of pipes needed based on the count of command nodes.
 */
int	pipecount(t_global *data)
{
	int	result;

	result = 0;
	while (data->hashtable[result])
	{
		result++;
	}
	return (result - 1);
}

/**
 * Cleans up resources and exits the process when a child process needs to quit.
 *
 * @param[in] args Pointer to the array of command arguments.
 */
static void	quit_child(char *(*args)[TABLE_SIZE])
{
	rl_clear_history();
	free_redir_args(args);
	close_all_fds();
	exit(EXIT_FAILURE);
}

/**
 * Handles input and output redirections for commands involved in piping.
 *
 * @param[in, out] redirects Array of redirection commands.
 */
void	handle_redirects_for_pipes(char *(*redirects)[TABLE_SIZE])
{
	int	i;

	i = 0;
	while ((*redirects)[i])
	{
		if ((*redirects)[i][0] == '<')
		{
			if (redirect_input((*redirects)[i], i) == 0)
				quit_child(redirects);
		}
		if ((*redirects)[i][0] == '>')
		{
			if (redirect_output((*redirects)[i]) == 0)
				quit_child(redirects);
		}
		i++;
	}
}

/**
 * Manages file descriptors for processes in a pipeline.
 *
 * @param[in] original_fd_out Original standard output file descriptor.
 * @param[in, out] data Pointer to the global data structure.
 * @param[in] current_node Current command node being processed.
 * @param[in] hashtable Array of all command nodes.
 */
void	handle_pipe(int original_fd_out, t_global *data,
	t_tkn *current_node, t_tkn **hashtable)
{
	t_bool		is_first_node;
	t_bool		has_next_node;
	t_tkn		*last_node;
	static int	pipe_fds[2];

	last_node = hashtable[pipecount(data)];
	is_first_node = (current_node == hashtable[0]);
	has_next_node = (current_node != last_node);
	if (!is_first_node)
		redirect_fd(pipe_fds[IN], STDIN_FILENO);
	if (has_next_node)
	{
		if (pipe(pipe_fds) == -1)
			ft_dprintf(2, "minishell: pipe: %s\n", strerror(errno));
		redirect_fd(pipe_fds[OUT], STDOUT_FILENO);
	}
	else
		redirect_fd(original_fd_out, STDOUT_FILENO);
}

// File: src/expand/expander.c
#include "../../includes/minishell.h"

void	handle_expand_fail(t_tkn **node);
char	*search_value(t_global **data, char **str, int i, int len);

static char	*prepare_new_content(t_tkn *node, int len,
	int var_len, char *exp_value)
{
	char	*new_content;
	int		token_len;
	int		value_len;
	int		i;

	value_len = ft_strlen(exp_value);
	token_len = ft_strlen(node->content) - var_len;
	new_content = ft_calloc((token_len + value_len + 1), sizeof(char));
	if (!new_content)
		return (NULL);
	i = 0;
	while (node->content[i] != '$')
	{
		new_content[i] = node->content[i];
		i++;
	}
	ft_strlcpy(new_content + i, exp_value, value_len + 1);
	while (node->content[len])
	{
		new_content[i + value_len] = node->content[len];
		i++;
		len++;
	}
	return (new_content);
}

static void	update_node(t_tkn **node, int len, int var_len, char **exp_value)
{
	char	*new_content;

	new_content = prepare_new_content(*node, len, var_len, *exp_value);
	if (new_content == NULL)
		return ;
	free((*node)->content);
	free(*exp_value);
	(*node)->content = ft_strdup(new_content);
	free(new_content);
}

static int	get_var_value(t_tkn **node, int i, t_global **data)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (is_var_name(node, i, len))
		len++;
	if (is_special_variable((*node)->content, TRUE))
		return (0);
	if (((*node)->content[i] == '?' && !(*node)->content[i + 1])
		|| !ft_strncmp((*node)->content + i, "? ", 2))
		value = ft_itoa((*data)->ret);
	else
		value = search_value(data, &(*node)->content, i, len);
	if (!value)
	{
		handle_expand_fail(node);
		if ((*node)->prev && (ft_strcmp((*node)->prev->content, ">") == 0
				|| ft_strcmp((*node)->prev->content, "<") == 0))
			return (0);
	}
	else
		update_node(node, i + len, len, &value);
	return (1);
}

int	check_if_expandable(t_tkn **node, t_global **data)
{
	int		i;
	int		result;

	result = 1;
	if (!ft_strcmp((*node)->type, VARIABLE)
		|| !ft_strcmp((*node)->type, STRING_STD)
		|| !ft_strcmp((*node)->type, EXPT_VARIABLE))
	{
		i = 0;
		while (i < (int)ft_strlen((*node)->content))
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				result = get_var_value(node, i, data);
			}
			i++;
		}
	}
	return (result);
}

int	expand(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data)
{
	int		i;
	int		result;
	t_tkn	*temp;

	i = 0;
	result = 1;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			result = check_if_expandable(&(*hashtable)[i], data);
			if (result == 0)
			{
				(*hashtable)[i] = temp;
				return (result);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return (result);
}

// File: src/expand/expander_utils2.c
#include "../includes/minishell.h"

/**
 * Searches for the key that matches the
 * token's variable and then returns its value.
 */
static char	*fetch_in_array(char **str, int i, int len, char *arr)
{
	char	*value;
	int		j;

	j = 0;
	if (ft_strncmp((*str) + i, arr, len) == 0)
	{
		while (arr[j] != '=')
			j++;
		if (j == len)
		{
			j++;
			value = ft_strdup(arr + j);
			return (value);
		}
	}
	return (NULL);
}

char	*search_value(t_global **data, char **str, int i, int len)
{
	char	*value;
	int		j;

	value = NULL;
	j = 0;
	while (!value && (*data)->env[j])
	{
		value = fetch_in_array(str, i, len, (*data)->env[j]);
		j++;
	}
	j = 0;
	if ((*data)->exported)
	{
		while (!value && (*data)->exported[j])
		{
			value = fetch_in_array(str, i, len, (*data)->exported[j]);
			j++;
		}
	}
	return (value);
}

void	handle_expand_fail(t_tkn **node)
{
	t_tkn	*temp;

	temp = *node;
	while (temp->prev)
		temp = temp->prev;
	if (ft_strcmp(temp->content, "echo") == 0)
	{
		free((*node)->content);
		temp = (*node)->prev;
		temp->next = (*node)->next;
		if ((*node)->next)
			(*node)->next->prev = temp;
		if (temp->space_after != (*node)->space_after)
			temp->space_after = (*node)->space_after;
		free(*node);
		*node = temp;
	}
	if ((*node)->prev && (ft_strcmp((*node)->prev->content, ">") == 0
			|| ft_strcmp((*node)->prev->content, "<") == 0))
		ft_printf("\n%sminishell: %s: ambiguous redirect%s\n",
			RED, (*node)->content, END);
}

// File: src/expand/expander_utils.c
#include "../includes/minishell.h"

/**
 * Determines if a character is a special variable character.
 *
 * @param c The character to check.
 * @return True if it is a special variable character, otherwise false.
 */
t_bool	is_special_var_char(char c)
{
	if ((c >= 32 && c <= 47 && c != 35) || (c >= 58 && c <= 64 && c != 63)
		|| (c >= 93 && c <= 96 && c != '_') || (c >= 123 && c <= 126))
	{
		return (true);
	}
	return (false);
}

/**
 * Checks if a variable is a special shell variable.
 *
 * @param var The variable to check.
 * @param print Whether to print a message if the variable is special.
 * @return True if the variable is special, otherwise false.
 */
t_bool	is_special_variable(char *var, t_bool print)
{
	if (!ft_strcmp(var, "$#") || !ft_strcmp(var, "$!") || !ft_strcmp(var, "$@")
		|| !ft_strcmp(var, "$$") || !ft_strcmp(var, "$0")
		|| !ft_strcmp(var, "$-") || !ft_strcmp(var, "$*"))
	{
		if (print)
			ft_dprintf(2, "%s\nThis functionality is beyond Minishell's \
			scope, ****@#$@***.\n\n%s", RED, END);
		return (true);
	}
	return (false);
}

/**
 * Determines if the content at a specific position in a token node is a 
 * variable.
 *
 * @param node The token node.
 * @param i The index in the content.
 * @param len The length after index to check.
 * @return True if it's a variable name, otherwise false.
 */
t_bool	is_var_name(t_tkn **node, int i, int len)
{
	if (((*node)->type[0] == 'v' && (*node)->content[i + len]
			&& !is_special_var_char((*node)->content[i + len]))
		|| ((*node)->type[0] == 's' && (*node)->content[i + len]
			&& !is_special_var_char((*node)->content[i + len])))
		return (TRUE);
	if (!ft_strcmp((*node)->type, EXPT_VARIABLE) && (*node)->content[i + len]
		&& !is_special_var_char((*node)->content[i + len]))
		return (TRUE);
	return (FALSE);
}

/**
 * Adds a new token node before the specified node.
 *
 * @param node The current node to insert before.
 * @param i The index in the content up to which the new node will include.
 */
void	add_node_before(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return ;
	new_node->content = ft_strndup((*node)->content, i - 1);
	new_node->type = STRING_STD;
	new_node->next = (*node);
	new_node->prev = (*node)->prev;
	(*node)->prev->next = new_node;
}

/**
 * Adds a new token node after the specified node.
 *
 * @param node The current node to insert after.
 * @param i The index in the content from which the new node will start.
 * @return The new node or NULL if allocation fails.
 */
t_tkn	*add_node_after(t_tkn **node, int i)
{
	t_tkn	*new_node;

	new_node = ft_calloc(1, sizeof(t_tkn));
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup((*node)->content + i);
	new_node->type = STRING_STD;
	new_node->next = (*node)->next;
	new_node->prev = *node;
	if ((*node)->next)
		(*node)->next->prev = new_node;
	return (new_node);
}

// File: src/expand/heredoc_expander.c
#include "../../includes/minishell.h"

static void	update_line(char **line, int len, char **value)
{
	char	*new_line;
	char	*temp;
	int		line_len;

	line_len = 0;
	temp = NULL;
	while ((*line)[line_len] != '$')
		line_len++;
	new_line = ft_strndup((*line), line_len);
	if ((*line)[line_len] == '$')
	{
		if (*value)
			temp = ft_strjoin(new_line, *value);
		else
			temp = ft_strdup(new_line);
		free(new_line);
		new_line = ft_strjoin(temp, (*line + line_len + len + 1));
	}
	free(temp);
	free(*line);
	free(*value);
	*line = ft_strdup(new_line);
	free(new_line);
}

int	get_var_value1(t_global **data, char **line, int i)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while ((*line)[i + len] && !is_special_var_char((*line)[i + len]))
		len++;
	if (((*line)[i] == '?' && !(*line)[i + 1])
			|| !ft_strncmp((*line) + i, "? ", 2))
		value = ft_itoa((*data)->ret);
	else if (is_special_variable(*line, FALSE))
	{
		printf("%s\nThis functionality is beyond \
		Minishell's scope.\n\n%s", RED, END);
		return (0);
	}
	else
		value = search_value(data, line, i, len);
	update_line(line, len, &value);
	return (1);
}

void	expand_heredoc(t_global *data, char **line)
{
	int		i;

	i = 0;
	while (i < (int)ft_strlen(*line))
	{
		if ((*line)[i] == '$')
		{
			i++;
			get_var_value1(&data, line, i);
		}
		i++;
	}
}

// File: src/lexer/lexer.c
#include "../includes/minishell.h"

/**
 * Checks if the input at a specific index starts with a valid command.
 *
 * @param[in] node Pointer to the token node being checked.
 * @param[in] i Index in the token array.
 * @return 1 if valid command found, 0 otherwise, with error message output.
 */
int	check_valid_input(t_tkn **node)
{
	if ((is_double_special_token((*node)) && !(*node)->delimiter)
		|| (!(*node)->next && !ft_strcmp((*node)->type, SPECIAL_CHAR)
			&& !(*node)->delimiter) || is_and_or((*node)->content))
	{
		ft_dprintf(2, "Syntax error.\n");
		return (0);
	}
	return (1);
}

/**
 * Traverses through token nodes and processes redirections.
 *
 * @param[in, out] node Pointer to the current token node to check.
 */
void	check_redirects(t_tkn **node)
{
	t_tkn	*head;

	head = *node;
	while (*node)
	{
		while ((*node)->content && is_redir((*node)->content)
			&& (*node)->next)
		{
			parse_redir(node, &head);
		}
		*node = (*node)->next;
	}
	*node = head;
}

/**
 * Performs lexical analysis on a hashtable of tokens, checking commands
 * and processing redirections.
 *
 * @param[in, out] hashtable Pointer to the hashtable of token nodes.
 * @return 1 on successful analysis, 0 on finding an error.
 */
int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int		i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!check_valid_input(&(*hashtable)[i]))
			{
				(*hashtable)[i] = temp;
				return (0);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		remove_pipe(&(*hashtable)[i], i);
		check_redirects(&(*hashtable)[i]);
		i++;
	}
	return (1);
}

// File: src/lexer/lexer_utils2.c
#include "../includes/minishell.h"

t_bool	is_redir_in(char *c)
{
	if (c && !ft_strcmp(c, LESS_THAN))
		return (true);
	return (false);
}

t_bool	is_redir_out(char *c)
{
	if (c && !ft_strcmp(c, GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_append(char *c)
{
	if (c && !ft_strcmp(c, DOUBLE_GREATER_THAN))
		return (true);
	return (false);
}

t_bool	is_heredoc(char *content)
{
	if (content && strcmp(content, DOUBLE_LESS_THAN) == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	is_redir(char *sig)
{
	if (is_redir_in(sig) || is_redir_out(sig)
		|| is_heredoc(sig) || is_append(sig))
		return (true);
	return (false);
}

// File: src/lexer/lexer_utils.c
#include "../includes/minishell.h"

// t_bool	input_starts_with_command(t_tkn	*node, int i)
// {
// 	if ((i == 0 && !node->prev
// 			&& !is_redir(node->content) && ft_strcmp(node->type, COMMAND))
// 		|| (i > 0 && node->prev && is_pipe(node->prev->content)
// 			&& ft_strcmp(node->type, COMMAND) && !is_redir_in(node->content)))
// 		return (FALSE);
// 	return (TRUE);
// }

void	remove_pipe(t_tkn **node, int i)
{
	t_tkn	*temp;

	if (i > 0 && is_pipe((*node)->content))
	{
		temp = (*node)->next;
		free((*node)->content);
		free(*node);
		*node = temp;
		if (*node)
			(*node)->prev = NULL;
	}
}

// File: src/lexer/parse_redir.c
#include "../../includes/minishell.h"

void	init_redir_args(char *(*args)[TABLE_SIZE]);
void	check_heredoc(t_tkn **node);
void	update_redir_files_list(char *(*redir)[TABLE_SIZE],
			char *sig, char *new_arg);

static void	free_1st_content_and_delimiter(t_tkn **node)
{
	free((*node)->content);
	(*node)->content = NULL;
	if ((*node)->delimiter)
		free((*node)->delimiter);
}

static void	handle_no_prev_node(t_tkn **node)
{
	t_tkn	*temp;

	free_1st_content_and_delimiter(node);
	if ((*node)->next && (*node)->next->next)
	{
		temp = (*node)->next;
		ft_memdel(*node);
		*node = temp;
		temp = (*node)->next;
		ft_memdel((*node)->content);
		ft_memdel(*node);
		temp->prev = NULL;
	}
	else
	{
		temp = *node;
		ft_memdel((*node)->next->content);
		ft_memdel((*node)->next);
		temp->next = NULL;
	}
	if (temp->next)
		temp->next->prev = temp;
	*node = temp;
}

static void	handle_prev_node(t_tkn **node)
{
	t_tkn	*temp;

	temp = (*node)->prev;
	if ((*node)->delimiter && temp->delimiter)
		temp->delimiter = ft_strdup((*node)->delimiter);
	if ((*node)->delimiter)
		free((*node)->delimiter);
	*node = (*node)->next;
	free((*node)->prev->content);
	free((*node)->prev);
	free((*node)->content);
	temp->next = (*node)->next;
	free(*node);
	if (temp->next)
		temp->next->prev = temp;
	*node = temp;
}

static void	update_node_after_redir(t_tkn **node)
{
	if ((*node)->prev)
		handle_prev_node(node);
	else
		handle_no_prev_node(node);
}

void	parse_redir(t_tkn **node, t_tkn **head)
{
	int		i;
	t_tkn	*temp;

	i = 0;
	temp = *node;
	if (is_heredoc((*node)->content))
		check_heredoc(node);
	while (!ft_strcmp((*head)->content, temp->content)
		&& is_redir(temp->content) && temp->next && temp->next->next)
	{
		*head = temp->next->next;
		temp = *head;
	}
	if (!(*head)->redir[0])
		init_redir_args(&(*head)->redir);
	update_redir_files_list(&(*head)->redir,
		(*node)->content, (*node)->next->content);
	if (is_heredoc((*node)->content))
		update_node_after_redir(node);
	else
		update_node_after_redir(node);
}

// File: src/lexer/parse_redir_utils.c
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

// File: src/main/exit_status.c
#include "../../includes/minishell.h"

int	is_folder(char *command)
{
	struct stat	statbuf;

	if (stat(command, &statbuf) != 0)
		return (0);
	if (S_ISDIR(statbuf.st_mode))
	{
		if (*command == '.')
			command++;
		if (*command == '.')
			command++;
		if (*command == '/')
			return (TRUE);
	}
	return (FALSE);
}

void	external_exit(int exit_status)
{
	rl_clear_history();
	close_all_fds();
	exit(exit_status);
}

// File: src/main/main.c
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
	data->original_stdin = STDIN_FILENO;
	data->original_stdout = STDOUT_FILENO;
	data->env = ft_strarr_dup(__environ);
	data->exported = NULL;
	data->open_redirs = 0;
	data->usr = ft_strdup(ft_getenv("USER", &data));
	init_hashtable(&data->hashtable);
	return (data);
}

int	main(void)
{
	static t_global	*data;
	int				result;

	data = init_data();
	data->usr_input = NULL;
	while (!data->exit)
	{
		data->sig_env = PROMPT;
		define_prompt_signals(&data);
		if (!data->exit && handle_prompt(&data) == -1)
			break ;
		clean_input_and_hashtable(&data);
	}
	result = data->ret;
	clean_stuff(&data);
	return (result);
}

// File: src/main/main_utils.c
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

// File: src/main/prompt.c
#include "../includes/minishell.h"

char	*cwd_error(void)
{
	static char	prompt[PATH_MAX];

	ft_bzero(prompt, sizeof(prompt));
	ft_strlcat(prompt, "minishell: ", PATH_MAX);
	ft_strlcat(prompt, strerror(errno), PATH_MAX);
	return (prompt);
}

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

// File: src/parse/parser.c
#include "../includes/minishell.h"

char	*get_tkn_type(t_tkn *node)
{
	if (!node->type)
	{
		if (node->content[0] == SIMPLE_QUOTE)
			return (STRING_STRONG);
		else if (node->content[0] == DOUBLE_QUOTE)
			return (STRING_STD);
		else if (is_special_token(node->content))
			return (SPECIAL_CHAR);
		else if ((node->content[0] == '$'
				&& identifier_is_valid(node->content + 1))
			|| !strcmp(node->content, "$?")
			|| is_special_variable(node->content, FALSE))
			return (VARIABLE);
		else if (is_export_var(node->content))
			return (EXPT_VARIABLE);
		else if (!node->prev || is_pipe(node->prev->content))
			return (COMMAND);
		else
			return (ARGUMENT);
	}
	return (node->type);
}

void	remove_quotes(char **content)
{
	char	*new_content;
	int		len;
	int		i;

	len = ft_strlen(*content) - 1;
	if (ft_strcmp(*content, PIPE)
		&& ((*content)[0] == 34 || (*content)[0] == 39))
	{
		new_content = ft_calloc(len, sizeof(char));
		i = 1;
		while (i < len)
		{
			new_content[i - 1] = (*content)[i];
			i++;
		}
		ft_strlcpy(*content, new_content, len);
		free(new_content);
	}
}

void	handle_export(t_tkn **node)
{
	char	*new_content;
	t_tkn	*head;
	t_tkn	*temp;

	new_content = NULL;
	head = *node;
	*node = (*node)->next;
	while ((*node) && is_export_var((*node)->content) && (*node)->next)
	{
		new_content = ft_strjoin((*node)->content, (*node)->next->content);
		free((*node)->content);
		(*node)->content = ft_strdup(new_content);
		temp = (*node)->next->next;
		if (temp)
			temp->prev = *node;
		free(new_content);
		free((*node)->next->content);
		free((*node)->next);
		(*node)->next = temp;
		*node = (*node)->next;
	}
	*node = head;
}

int	parse(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data)
{
	int		i;
	int		syntax;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			(*hashtable)[i]->type = get_tkn_type((*hashtable)[i]);
			remove_quotes(&(*hashtable)[i]->content);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		if (!ft_strcmp((*hashtable)[i]->content, "export"))
			handle_export(hashtable[i]);
		i++;
	}
	syntax = 0;
	if (expand(hashtable, data) == 1)
		syntax = lexer(hashtable);
	return (syntax);
}

// File: src/parse/parser_utils2.c
#include "../../includes/minishell.h"

t_bool	is_export_var(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '=' && !content[i + 1])
			return (true);
		i++;
	}
	return (false);
}

t_bool	check_there_is_var(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '$' && (ft_isalpha(content[i + 1])
				|| content[i + 1] == '_'))
			return (true);
		i++;
	}
	return (false);
}

t_bool	is_empty_str(char *content, char quote)
{
	int	i;

	i = 1;
	while (content[i] && content[i] != quote)
	{
		if (content[i] != 32)
			return (false);
		i++;
	}
	return (true);
}

// File: src/parse/parser_utils.c
#include "../includes/minishell.h"

t_bool	is_pipe(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0))
		return (true);
	return (false);
}

t_bool	is_and_or(char *token)
{
	if (token && (ft_strcmp(token, "&&") == 0
			|| ft_strcmp(token, "||") == 0))
		return (true);
	return (false);
}

t_bool	is_pipe_and_or(char *token)
{
	if (token && (ft_strcmp(token, PIPE) == 0
			|| ft_strcmp(token, "&&") == 0
			|| ft_strcmp(token, "||") == 0))
		return (true);
	return (false);
}

t_bool	is_special_token(char *token)
{
	if (token
		&& ((ft_strncmp(token, PIPE, 1) == 0)
			|| (ft_strncmp(token, AMPERSAND, 1) == 0)
			|| (ft_strncmp(token, GREATER_THAN, 1) == 0)
			|| (ft_strncmp(token, LESS_THAN, 1) == 0)
			|| (ft_strncmp(token, "||", 2) == 0)
			|| (ft_strncmp(token, "&&", 2) == 0)
			|| (ft_strncmp(token, DOUBLE_GREATER_THAN, 2) == 0)
			|| (ft_strncmp(token, DOUBLE_LESS_THAN, 2) == 0)))
		return (true);
	else
		return (false);
}

t_bool	is_double_special_token(t_tkn *node)
{
	if ((!node->next || (node->next && is_special_token(node->next->content)))
		&& is_special_token(node->content) && node->next
		&& (!is_pipe(node->content) && !is_redir_in(node->next->content)))
		return (true);
	return (false);
}

// File: src/redirections/handle_redir.c
#include "../../includes/minishell.h"

void	save_original_fd_in(int original_fds[2])
{
	if (original_fds[IN] == -1)
		original_fds[IN] = dup(STDIN_FILENO);
}

void	save_original_fd_out(int original_fds[2])
{
	if (original_fds[OUT] == -1)
		original_fds[OUT] = dup(STDOUT_FILENO);
}

int	handle_input_redirect(char *redir, int here_num, int original_fds[2])
{
	save_original_fd_in(original_fds);
	if (redirect_input(redir, here_num) == 0)
	{
		redirect_fd(original_fds[IN], STDIN_FILENO);
		return (0);
	}
	return (1);
}

int	handle_output_redirect(char *redir, int original_fds[2])
{
	save_original_fd_out(original_fds);
	if (redirect_output(redir) == 0)
	{
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
		return (0);
	}
	return (1);
}

int	handle_redirects(t_global *data, int ori_fds[2])
{
	int	i;

	i = 0;
	ori_fds[IN] = -1;
	ori_fds[OUT] = -1;
	while (data->hashtable[0]->redir[i])
	{
		if (data->hashtable[0]->redir[i][0] == '<')
		{
			if (!handle_input_redirect(data->hashtable[0]->redir[i],
					i, ori_fds))
				return (0);
		}
		if (data->hashtable[0]->redir[i][0] == '>')
		{
			if (!handle_output_redirect(data->hashtable[0]->redir[i], ori_fds))
				return (0);
		}
		i++;
	}
	return (1);
}

// File: src/redirections/redir.c
#include "../../includes/minishell.h"

void	write_in_heredoc(t_global *data, int heredoc_number,
			int hash_number, char *eof)
{
	int		tmp_file_fd;
	char	*filename;
	char	*line;

	tmp_file_fd = 0;
	(void)hash_number;
	filename = tmp_filename(heredoc_number);
	tmp_file_fd = get_fd(filename);
	line = readline("> ");
	while (line && ft_strncmp(line, eof, (ft_strlen(eof) + 1)))
	{
		expand_heredoc(data, &line);
		ft_putstr_fd(line, tmp_file_fd);
		ft_putchar_fd('\n', tmp_file_fd);
		free(line);
		line = readline("> ");
	}
	if (!line)
		handle_heredoc_ctrl_d(data, eof);
	close(tmp_file_fd);
	free(filename);
	free(line);
}

int	redirect_input(char *input_redirect, int i)
{
	int		fd;
	char	*filename;

	if (input_redirect[1] == '<')
	{
		filename = tmp_filename(i);
		fd = open(filename, O_RDONLY, FD_CLOEXEC);
		if (fd == -1)
		{
			ft_dprintf(STDERR_FILENO, "Fail to open heredoc: %s: %s\n",
				&input_redirect[2], strerror(errno));
			free(filename);
			return (0);
		}
		free(filename);
	}
	else
	{
		fd = open(&input_redirect[2], O_RDONLY, FD_CLOEXEC);
		if (fd == -1)
		{
			ft_dprintf(STDERR_FILENO, "open: %s: %s\n",
				&input_redirect[2], strerror(errno));
			return (0);
		}
	}
	redirect_fd(fd, STDIN_FILENO);
	return (1);
}

int	redirect_output(char *output_redirect)
{
	int	fd;

	if (output_redirect[1] == '>')
		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(&output_redirect[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s\n", strerror(errno));
		return (0);
	}
	else
	{
		redirect_fd(fd, STDOUT_FILENO);
	}
	return (1);
}

// File: src/redirections/redir_utils2.c
#include "../../includes/minishell.h"

void	close_extra_fds(void)
{
	int	last_open_fd;

	last_open_fd = open("/tmp/last_fd", O_RDWR | O_CREAT, 0666);
	if (last_open_fd == -1)
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
			"/tmp/last_fd", strerror(errno));
	while (last_open_fd > STDERR_FILENO)
	{
		close(last_open_fd);
		last_open_fd--;
	}
}

void	close_all_fds(void)
{
	close_extra_fds();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	redirect_fd(int fd_to_redirect, int fd_location)
{
	dup2(fd_to_redirect, fd_location);
	close(fd_to_redirect);
}

void	redirect_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
		redirect_fd(fd_in, STDIN_FILENO);
	if (fd_out != STDOUT_FILENO)
		redirect_fd(fd_out, STDOUT_FILENO);
}

void	restore_original_fds(int original_fds[2])
{
	if (original_fds[IN] != -1)
		redirect_fd(original_fds[IN], STDIN_FILENO);
	if (original_fds[OUT] != -1)
		redirect_fd(original_fds[OUT], STDOUT_FILENO);
}

// File: src/redirections/redir_utils.c
#include "../../includes/minishell.h"

char	*tmp_filename(int heredoc_number)
{
	char	filename[30];
	char	*number_str;

	ft_bzero(filename, 30);
	number_str = ft_itoa(heredoc_number);
	ft_strlcat(filename, ".tmp/heredoc", 30);
	ft_strlcat(filename, number_str, 30);
	free(number_str);
	return (ft_strdup(filename));
}

t_bool	ends_with_linebreak(char *line)
{
	int	i;

	i = 0;
	while (line[i + 1] != '\0')
		i++;
	if (line[i] == '\n')
		return (TRUE);
	return (FALSE);
}

int	get_fd(char *filename)
{
	int	tmp_file_fd;

	tmp_file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_file_fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "open: %s: %s\n", filename, strerror(errno));
		free(filename);
		return (-1);
	}
	return (tmp_file_fd);
}

void	handle_heredoc_ctrl_d(t_global *data, char *eof)
{
	ft_dprintf(STDOUT_FILENO,
		"%sminishell: warning: heredoc delimited by EOF (wanted '%s')%s\n",
		YELLOW, eof, END);
	data->ret = 0;
}

// File: src/signals/signals.c
#include "minishell.h"

int	g_sig = 0;

static void	handle_sigint(int sig)
{
	(void)sig;
	g_sig = 1;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	define_prompt_signals(t_global **data)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_handler = &handle_sigint;
	sa_sigint.sa_flags = 0;
	(void)data;
	sigemptyset(&sa_sigint.sa_mask);
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

void	define_exec_signals(int pid)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);
	if (pid == 0)
		sa_sigint.sa_handler = SIG_DFL;
	else
		sa_sigint.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	if (pid == 0)
		sa_sigquit.sa_handler = SIG_DFL;
	else
		sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

void	define_heredoc_signals(int pid)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sa_sigint.sa_flags = 0;
	sigemptyset(&sa_sigint.sa_mask);
	if (pid == 0)
		sa_sigint.sa_handler = SIG_DFL;
	else
		sa_sigint.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

// File: src/token/token.c
#include "../includes/minishell.h"

/**
 * Checks for closed quotes starting from a given index in a string.
 *
 * @param[in] input The input string to check.
 * @param[in] i The index to start checking from.
 * @return Length of the quoted segment if properly closed, -1 if not closed,
 *         0 if no quotes are found.
 */
int	check_quotes(char *input, int i)
{
	int		len;
	char	start;

	len = 0;
	if (input[i] == SIMPLE_QUOTE)
		start = SIMPLE_QUOTE;
	if (input[i] == DOUBLE_QUOTE)
		start = DOUBLE_QUOTE;
	i++;
	len++;
	while (input[i] && start != input[i])
	{
		len++;
		i++;
	}
	if (!input[i])
		return (-1);
	if (input[i] == start)
		return (len + 1);
	return (0);
}

/**
 * Determines if a character is considered special character for tokenization.
 *
 * @param[in] c The character to evaluate.
 * @return True if the character is special ('|', '&', '<', '>'),
 *  false otherwise.
 */
t_bool	is_special_char(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>')
		return (true);
	return (false);
}

/**
 * Calculates the length of the token starting from the specified index.
 *
 * @param[in] input The string to parse.
 * @param[in] i The start index for token measurement.
 * @return The length of the token.
 */
int	get_token_len(char *input, int i)
{
	int	len;

	len = 0;
	if (is_special_char(input[i]))
	{
		len++;
		if (input[i + 1] && input[i + 1] == input[i])
			len++;
	}
	else if (input[i] == SIMPLE_QUOTE || input[i] == DOUBLE_QUOTE)
		len = check_quotes(input, i);
	else
	{
		if (input[i + len] == '$')
			len++;
		while (input[i + len] && input[i + len] != ' ' && input[i + len] != '$'
			&& !is_special_char(input[i + len])
			&& input[i + len] != SIMPLE_QUOTE
			&& input[i + len] != DOUBLE_QUOTE)
			len++;
	}
	return (len);
}

int	error_open_quote(void)
{
	printf("Error: open quote.\n");
	return (0);
}

/**
 * Tokenizes the user input stored in global data.
 *
 * @param[in, out] data The global data structure containing user input.
 * @return 1 on successful tokenization, 0 on error (e.g., open quote).
 */
int	tokenize(t_global **data)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while ((*data)->usr_input[i])
	{
		while ((*data)->usr_input[i] == ' ')
			i++;
		len = get_token_len((*data)->usr_input, i);
		if (len == -1)
		{
			printf("Error: open quote.\n");
			return (0);
		}
		if (len > 0)
			populate_hashtable(data, i, len);
		i += len;
	}
	if (len == 0)
		return (0);
	return (1);
}

// File: src/token/token_utils.c
#include "../includes/minishell.h"

void	init_hashtable(t_tkn *(*hashtable)[TABLE_SIZE])
{
	int	i;

	i = 0;
	while (i < TABLE_SIZE)
	{
		(*hashtable)[i] = NULL;
		i++;
	}
	return ;
}

char	*get_token(char *input, int i, int len)
{
	char	*token;

	token = ft_calloc(len + 1, sizeof(char));
	if (!token)
		return (NULL);
	while (--len >= 0)
		token[len] = input[i + len];
	return (token);
}

t_tkn	*add_node(t_tkn **tkn_node, char **content)
{
	t_tkn	*new_node;
	t_tkn	*temp;

	new_node = ft_calloc(1, sizeof(t_tkn));
	temp = NULL;
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup(*content);
	new_node->space_after = TRUE;
	new_node->next = NULL;
	if (!(*tkn_node))
		(*tkn_node) = new_node;
	else
	{
		temp = (*tkn_node);
		while ((*tkn_node)->next != NULL)
			(*tkn_node) = (*tkn_node)->next;
		(*tkn_node)->next = new_node;
		new_node->prev = (*tkn_node);
		*tkn_node = temp;
	}
	return (new_node);
}

void	check_non_spaced_var(char *input, int i, t_tkn **node)
{
	t_tkn	*temp;

	if ((*node) && input[i - 1] != ' ')
	{
		temp = *node;
		while ((*node)->next != NULL)
			*node = (*node)->next;
		(*node)->space_after = FALSE;
		*node = temp;
	}
}

void	populate_hashtable(t_global **data, int idx, int len)
{
	char	*token;
	int		i;

	token = get_token((*data)->usr_input, idx, len);
	i = 0;
	if (ft_strncmp(token, PIPE, 1) == 0)
	{
		while ((*data)->hashtable[i])
			i++;
	}
	else
	{
		check_non_spaced_var((*data)->usr_input, idx, &(*data)->hashtable[i]);
		while ((*data)->hashtable[i + 1] != NULL)
			i++;
	}
	add_node(&(*data)->hashtable[i], &token);
	free(token);
}

