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

bool	is_special_char(char c)
{
	if ((c >= 32 && c <= 47) || (c >= 58 && c <= 64)
		|| (c >= 93 && c <= 96 && c != '_') || (c >= 123 && c <= 126))
	{
		return (true);
	}
	return (false);
}

void	update_list(t_tkn **node, int len, char **exp_value)
{
	t_tkn	*new_node;

	new_node = NULL;
	if ((*node)->content[len])
	{
		new_node = ft_calloc(1, sizeof(t_tkn));
		if (!new_node)
			return ;
		new_node->content = ft_strdup((*node)->content + len);
		new_node->type = STRING_STD;
		new_node->next = (*node)->next;
		new_node->prev = *node;
		if ((*node)->next)
			(*node)->next->prev = new_node;
	}
	free((*node)->content);
	(*node)->content = *exp_value;
	(*node)->next = new_node;
}

char	*get_var_value(t_tkn **node, int i, char **env)
{
	int		j;
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (((*node)->type[0] == 'v' && (*node)->content[i + len]
		&& !is_special_char((*node)->content[i + len]))
		|| ((*node)->type[0] == 's' && !is_special_char((*node)->content[i + len])))
		len++;
	j = 0;
	//printf ("ENTROU AQUI %d\n", len);
	while (env[j])
	{
		if (ft_strncmp((*node)->content + i, env[j], len) == 0)
		{
			value = fetch_on_env(env[j]);
			if (value)
			{
				update_list(node, i + len, &value);
				printf("var_value = %s\n", (*node)->content);
			}
		}
		j++;
	}
	return (NULL);
}

char	*check_if_expandable(t_tkn **node, char **env)
{
	int		i;
	char	*value;

	value = NULL;
	if (!ft_strcmp((*node)->type, VARIABLE) || !ft_strcmp((*node)->type, STRING_STD))
	{
		i = 0;
		while ((*node)->content[i] != '\0')
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				value = get_var_value(node, i, env);
				break ;
			}
			i++;
		}
	}
	return (value);
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


// TO TEST:
// echo "hello $USER"
// echo "hello $USER !!"
// echo 'hello $USER'
// echo '"hello $USER"'
// echo hello "'"$USER"'"
// echo "hello '$USER!' How are you?"
