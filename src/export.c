#include "../includes/minishell.h"

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

t_bool	identifier_is_valid(char *str)
{
	int		i;
	char	*key;

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
	return (TRUE);
}

void	replace_or_add(char *arg, t_global *data)
{
	char	*key;

	key = NULL;
	if (ft_strchr_i(arg, '=') != -1)
	{
		key = ft_strndup(arg, ft_strchr_i(arg, '=') + 1);
		ft_strarr_str_replace(data->env, key, arg);
		ft_strarr_str_replace(data->exported, key, arg);
	}
	else if (ft_strchr_i(arg, '=') == -1)
		key = ft_strdup(arg);
	if (ft_strarr_str(data->env, key) == -1
		&& ft_strarr_str(data->exported, key) == -1
		&& ft_strarr_str(__environ, key) == -1)
		ft_strarr_stradd(&data->exported, arg);
	if (ft_strarr_str(data->env, key) == -1
		&& ft_strarr_str(data->exported, key) == -1
		&& ft_strarr_str(__environ, key) != -1)
		ft_strarr_stradd(&data->env, arg);
	if (key != NULL)
		free(key);
}

int	ft_export(char **args, t_global *data)
{
	int		i;

	if (args)
	{
		i = 0;
		while (args[i])
		{
			if (!identifier_is_valid(args[i]))
				ft_printf("export: `%s': not a valid identifier\n", args[i]);
			else if (identifier_is_valid(args[i]))
				replace_or_add(args[i], data);
		i++;
		}
	}
	else
		export_no_args(data);
	return (0);
}

// ~/Documents/Projects/minishell/minishell (develop*) » export ""
// export: not valid in this context: 
