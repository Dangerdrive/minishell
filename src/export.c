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


void sort_env(t_global **data)
{
    int ordered;
    int i;
    int env_len;
    char *tmp;

    env_len = ft_strarray_len((*data)->env);
    ordered = 0;
	(*data)->sorted_env = ft_strarray_dup((*data)->env);
    while (!ordered)
	{
        ordered = 1;
        i = 0;
        while (i < env_len - 1) {
            if (ft_strcmp((*data)->sorted_env[i], (*data)->sorted_env[i + 1]) > 0) {
                tmp = (*data)->sorted_env[i];
                (*data)->sorted_env[i] = (*data)->sorted_env[i + 1];
                (*data)->sorted_env[i + 1] = tmp;
                ordered = 0;
            }
            i++;
        }
        env_len--;
    }
}

void		print_exp(char **sorted_env)
{
	int	i;

	i = 0;
	while (sorted_env[i])
	{
		ft_printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
}

int	export(char **args, t_global *data)
{
	if (args && args[1])
	{
		ft_strarraycat(args, data->env);
	}
	else
	{
		sort_env(&data);
		print_exp(data->sorted_env);
	}
	print_exp(data->env); //
	return (0);
}
//se o export for chamado sem argumentos, ele deve mostrar todas as variáveis de ambiente
//se o export for chamado com argumentos e atribuir valor, ele deve adicionar ou modificar a variável de ambiente
//se o export for chamado com argumentos sem atribuir valor

