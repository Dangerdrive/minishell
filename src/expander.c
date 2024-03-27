#include "../includes/minishell.h"

char	*get_value(char *env_value)
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

char	*get_var_value(char *var, char **env, char *type)
{
	int		i;
	int		len;
	char	*value;

	len = 0; // IT DOESN'T RECOGNIZE A VARIABLE AT THE END OF A STRING.
	while (var[len]
			&& ((type[0] == 'v' && var[len]) || (type[0] == 's' && var[len] != ' ')))
		len++;
	i = 0;
	printf("VAR::: %s	|	TYPE::: %s	|	LEN::: %d\n", var, type, len);
	while (env[i])
	{
		if (ft_strncmp(var, env[i], len) == 0)
		{
			value = get_value(env[i]);
			return (value);
		}
		i++;
	}
	return (NULL);
}

void	try_expand(t_tkn **node, char **env)
{
	int	i;

	if (!ft_strcmp((*node)->type, VARIABLE) || !ft_strcmp((*node)->type, STRING_STD))
	{
		i = 0;
		while ((*node)->content[i] != '\0')
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				(*node)->expanded = get_var_value((*node)->content + i, env, (*node)->type);
				printf("var_value = %s\n", (*node)->expanded);
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
			try_expand(&(*hashtable)[i], env);
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return ;
}
