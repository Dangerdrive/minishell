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


void		print_exp(char **sorted_env)
{
	char	*key;
	char	*value;
	int		i;
	int		delim_pos;

	i = 0;
	while (sorted_env && sorted_env[i])
	{
		delim_pos = ft_strchr_i(sorted_env[i], '=');
		if (delim_pos != -1)
		{
			value = ft_strdup(sorted_env[i] + delim_pos + 1);
			key = strndup(sorted_env[i], delim_pos - ft_strlen(sorted_env[i]) + 1);
			if (value) 
				ft_printf("declare -x %s=\"%s\"\n", key, value);
			else
				ft_printf("declare -x %s=\"\"\n", key);
		}
		else
			ft_printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
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

int	ft_export(char **args, t_global *data)
{
	char	**sorted_env;
	char	**sorted_export;
	int		i;

	if (args)
	{
		i = 0;
		while (args[i])
		{
			if (!validate_identifier(args[i]))
				ft_printf("export: `%s': not a valid identifier\n", args[i]);
			else if (((ft_strnstr(data-env, args[i], ft_strlen(args[i])) != NULL) || (ft_strnstr(data-exported, args[i], ft_strlen(args[i])) != NULL)) && (ft_strchr_i(args[i], '=') != -1))
			{
				//substituir valor
				//free(valorencontrado);
				valorencontrado = ft_strdup(args[i]);
			}
			else if (ft_strchr_i(args[i], '=') != -1)
				ft_strarr_stradd(&data->exported, args[i]);
		i++;
		}
	}
	else
	{
		sorted_env = ft_strarr_sort(data->env);
		sorted_export = ft_strarr_sort(data->exported);
		print_exp(sorted_env);
		print_exp(sorted_export);
	}
	return (0);
}


