include "../includes/minishell.h"

//teria que receber sorted_env
int			add_to_env(char *str, t_env *sorted_env)
{
	t_env	*new;
	t_env	*temp;

	if (env && sorted_env->value == NULL)
	{
		new->key = ft_strndup(str, ft_strchr_i(str, '='));
		new->value = ft_strdup(ft_strchr(str, '=') + 1);
		new->next = NULL;
		return (0);
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return (-1);
	new->key = ft_strndup(str, ft_strchr_i(str, '='));
	new->value = ft_strdup(ft_strchr(str, '=') + 1);
	new->next = NULL;
	while (env && sorted_env->next && sorted_env->next->next)
		env = sorted_env->next;
	temp = sorted_env->next;
	sorted_env->next = new;
	new->next = temp;
	return (0);
}

void		print_exp(t_env *sorted_env)
{
	while (env)
	{
		ft_printf("declare -x %s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	export(char *str, t_env *sorted_env)
{
	if (str)
	{
		if (add_to_env(str, sorted_env) == -1)
			return (-1);
	}
	else
		print_exp(sorted_env);
	return (0);
}
//se o export for chamado sem argumentos, ele deve mostrar todas as variáveis de ambiente
//se o export for chamado com argumentos e atribuir valor, ele deve adicionar ou modificar a variável de ambiente
//se o export for chamado com argumentos sem atribuir valor