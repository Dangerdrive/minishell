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
