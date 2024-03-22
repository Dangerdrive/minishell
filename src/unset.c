#include "minishell.h"

static size_t env_size(char *env)
{
    size_t i;

    i = 0;
    while (env[i] && env[i] != '=')
        i++;
    return (i);
}

static void free_node(t_env *env)
{
    ft_memdel(env->key);
    ft_memdel(env->value);
    ft_memdel(env);
}

/*adequar args para como receberemos argumentos*/
int ft_unset(char **args, t_global *data)
{
    t_env *prev;
    t_env *current;

    if (!args[1])
        return (0);
    prev = NULL;
    current = data->env;
    while (current)
    {
        if (ft_strncmp(args[1], current->key, env_size(current->key)) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                data->env = current->next;
            free_node(current);
            return (0);
        }
        prev = current;
        current = current->next;
    }
    return (0);
}