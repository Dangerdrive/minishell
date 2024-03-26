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

int	get_var_len(char *var)
{
	int	i;

	i = 0;
	while (var[i] != '\0' || var[i] != ' ')
		i++;
	return (i);
}

void	try_expand(t_tkn **node, char **env)
{
	int	i;
	int	j;
	int len;

	if (!ft_strcmp((*node)->type, VARIABLE) || !ft_strcmp((*node)->type, STRING_STD))
	{
		i = 0;
		len = 0;
		while ((*node)->content[i] != '\0')
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				j = 0;
				len = get_var_len((*node)->content + i);
				printf("LEN = %d\n", len);
				while (env[j])
				{
					if (ft_strncmp((*node)->content + i, env[j], len) == 0)
					{
						(*node)->expanded = get_value(env[j]);
						printf("var_value = %s\n", (*node)->expanded);
						return ;
					}
					j++;
				}
			}
			if (len)
				i += len - 1;
			else
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
