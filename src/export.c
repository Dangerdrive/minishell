#include "../includes/minishell.h"

//teria que receber sorted_env
// int			add_to_env(char *str, t_env *sorted_env)
// {
// 	t_env	*new;
// 	t_env	*temp;

// 	if (env && sorted_env->value == NULL)
// 	{
// 		new->key = ft_strndup(str, ft_strchr_i(str, '='));
// 		new->value = ft_strdup(ft_strchr(str, '=') + 1);
// 		new->next = NULL;
// 		return (0);
// 	}
// 	new = malloc(sizeof(t_env));
// 	if (!new)
// 		return (-1);
// 	new->key = ft_strndup(str, ft_strchr_i(str, '='));
// 	new->value = ft_strdup(ft_strchr(str, '=') + 1);
// 	new->next = NULL;
// 	while (env && sorted_env->next && sorted_env->next->next)
// 		env = sorted_env->next;
// 	temp = sorted_env->next;
// 	sorted_env->next = new;
// 	new->next = temp;
// 	return (0);
// }


void		print_exp(char **sorted_arr)
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
}

int validate_identifier(char *str)
{
	int i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (1);
	i++;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

void replace_or_add(char *arg, t_global *data)
{
	char *key;

	key = NULL;
	if (ft_strchr_i(arg, '=') != -1)
		key = ft_strndup(arg, ft_strchr_i(arg, '='));
	else if (ft_strchr_i(arg, '=') == -1)
		key = ft_strdup(arg);
	ft_strarr_str_replace(data->env, key, arg);
	ft_strarr_str_replace(data->exported, key, arg);
	free(key);
	if (ft_strarr_str(data->exported, key) == NULL 
		&& ft_strarr_str(data->exported, key) == NULL) //revisar se o certo é key mesmo, ou seria arg/ se faz diferença
		ft_strarr_stradd(&data->exported, arg);
}

int	ft_export(char **args, t_global *data)
{

	int		i;

	if (args)
	{
		i = 0;
		while (args[i])
		{
			if (!validate_identifier(args[i]))
				ft_printf("export: `%s': not a valid identifier\n", args[i]);
			else if (validate_identifier(args[i]))
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
