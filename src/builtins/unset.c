#include "minishell.h"

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

static int	key_matches_target(const char *strarr_elem, const char *tgt)
{
	char	*key;
	int		result;

	key = strndup(strarr_elem, ft_strchr_i(strarr_elem, '='));
	result = ft_strcmp(key, tgt);
	free(key);
	return (result == 0);
}

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
		}
	}
}

int	ft_unset(char **args, int args_len, t_global *data)
{
	int		i;
	int 	ret;

	ret = 0;
	i = 1;
	if (args_len <= 1)
	{
		ft_printf("unset: not enough arguments\n");
		ret = 1;
	}
	while (i < args_len)
	{
		if (!unset_identifier_is_valid(args[i]))
		{
			ft_printf("unset: `%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		else if (unset_identifier_is_valid(args[i]))
			remove_variable(args[i], data);
		i++;
	}
	return (ret);
}