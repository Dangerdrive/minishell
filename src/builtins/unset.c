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
				data->exported = NULL;
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
