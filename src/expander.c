#include "../includes/minishell.h"

char	*fetch_on_env(char *env_value)
{
	char	*result;

	while (*env_value != '=')
	{
		env_value++;
	}
	env_value++;
	result = ft_strdup(env_value);
	return (result);
}

char	*get_var_value(char **var, char **env, char *type)
{
	int		i;
	int		len;

	len = 0;
	while ((type[0] == 'v' && *var[len])
		|| ((type[0] == 's' && *var[len] != ' ') && (type[0] == 's' && *var[len] != '"')))
		len++;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(*var, env[i], len) == 0)
		{
			free(var);
			*var = fetch_on_env(env[i]);
			return (*var);
		}
		i++;
	}
	return (NULL);
}

void	check_if_expandable(t_tkn **node, char **env)
{
	int		i;

	if (!ft_strcmp((*node)->type, VARIABLE) || !ft_strcmp((*node)->type, STRING_STD))
	{
		i = 0;
		while ((*node)->content[i] != '\0')
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				(*node)->content = get_var_value(&(*node)->content + i, env, (*node)->type);
				printf("var_value = %s\n", (*node)->content);
			}
			i++;
		}
	}
	return ;
}

void	expand(t_tkn *(*hashtable)[TABLE_SIZE], char **env)
{
	int		i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			check_if_expandable(&(*hashtable)[i], env);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return ;
}
