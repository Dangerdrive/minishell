/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:59:38 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:59:39 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_expand_fail(t_tkn **node);
char	*search_value(t_global **data, char **str, int i, int len);

static char	*prepare_new_content(t_tkn *node, int len,
	int var_len, char *exp_value)
{
	char	*new_content;
	int		token_len;
	int		value_len;
	int		i;

	value_len = ft_strlen(exp_value);
	token_len = ft_strlen(node->content) - var_len;
	new_content = ft_calloc((token_len + value_len + 1), sizeof(char));
	if (!new_content)
		return (NULL);
	i = 0;
	while (node->content[i] != '$')
	{
		new_content[i] = node->content[i];
		i++;
	}
	ft_strlcpy(new_content + i, exp_value, value_len + 1);
	while (node->content[len])
	{
		new_content[i + value_len] = node->content[len];
		i++;
		len++;
	}
	return (new_content);
}

static void	update_node(t_tkn **node, int len, int var_len, char **exp_value)
{
	char	*new_content;

	new_content = prepare_new_content(*node, len, var_len, *exp_value);
	if (new_content == NULL)
		return ;
	free((*node)->content);
	free(*exp_value);
	(*node)->content = ft_strdup(new_content);
	free(new_content);
}

static int	get_var_value(t_tkn **node, int i, t_global **data)
{
	int		len;
	char	*value;

	len = 0;
	value = NULL;
	while (is_var_name(node, i, len))
		len++;
	if (is_special_variable((*node)->content, TRUE))
		return (0);
	if (((*node)->content[i] == '?' && !(*node)->content[i + 1])
		|| !ft_strncmp((*node)->content + i, "? ", 2))
		value = ft_itoa((*data)->ret);
	else
		value = search_value(data, &(*node)->content, i, len);
	if (!value)
	{
		handle_expand_fail(node);
		if ((*node)->prev && (ft_strcmp((*node)->prev->content, ">") == 0
				|| ft_strcmp((*node)->prev->content, "<") == 0))
			return (0);
	}
	else
		update_node(node, i + len, len, &value);
	return (1);
}

int	check_if_expandable(t_tkn **node, t_global **data)
{
	int		i;
	int		result;

	result = 1;
	if (!ft_strcmp((*node)->type, VARIABLE)
		|| !ft_strcmp((*node)->type, STRING_STD)
		|| !ft_strcmp((*node)->type, EXPT_VARIABLE))
	{
		i = 0;
		while (i < (int)ft_strlen((*node)->content))
		{
			if ((*node)->content[i] == '$')
			{
				i++;
				result = get_var_value(node, i, data);
			}
			i++;
		}
	}
	return (result);
}

int	expand(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data)
{
	int		i;
	int		result;
	t_tkn	*temp;

	i = 0;
	result = 1;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			result = check_if_expandable(&(*hashtable)[i], data);
			if (result == 0)
			{
				(*hashtable)[i] = temp;
				return (result);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		i++;
	}
	return (result);
}
