#include "../../includes/minishell.h"

// static void		print_error(char **args)
// {
// 	ft_putstr_fd("cd: ", 2);
// 	if (args[2])
// 		ft_putstr_fd("string not in pwd: ", 2);
// 	else
// 	{
// 		ft_putstr_fd(strerror(errno), 2);
// 		ft_putstr_fd(": ", 2);
// 	}
// 	ft_putendl_fd(args[1], 2);
// }

// static char		*get_env_path(t_env *env, const char *var, size_t len)
// {
// 	char	*oldpwd;
// 	int		i;
// 	int		j;
// 	int		s_alloc;

// 	while (env && env->next != NULL)
// 	{
// 		if (ft_strncmp(env->value, var, len) == 0)
// 		{
// 			s_alloc = ft_strlen(env->value) - len;
// 			if (!(oldpwd = malloc(sizeof(char) * s_alloc + 1)))
// 				return (NULL);
// 			i = 0;
// 			j = 0;
// 			while (env->value[i++])
// 			{
// 				if (i > (int)len)
// 					oldpwd[j++] = env->value[i];
// 			}
// 			oldpwd[j] = '\0';
// 			return (oldpwd);
// 		}
// 		env = env->next;
// 	}
// 	return (NULL);
// }

// void	replace_or_add_env(char *arg, t_global *data)
// {
// 	char	*key;

// 	key = NULL;
// 	if (ft_strchr_i(arg, '=') != -1)
// 	{
// 		key = ft_strndup(arg, ft_strchr_i(arg, '='));
// 		ft_strarr_str_replace(data->env, key, arg);
// 	}
// 	else if (ft_strchr_i(arg, '=') == -1)
// 		key = ft_strdup(arg);
// 	if (ft_strarr_str(data->env, key) == -1
// 		&& ft_strarr_str(data->env, key) == -1)
// 		ft_strarr_stradd(&data->env, arg);
// 	if (key != NULL)
// 		free(key);
// }

// static int		update_oldpwd(t_global *data)
// {
// 	char	cwd[4096];
// 	char	*oldpwd;

// 	if (getcwd(cwd, 4096) == NULL)
// 		return (1);
// 	if (!(oldpwd = ft_strjoin("OLDPWD=", cwd)))
// 		return (1);
// 	if (ft_strarr_str(data->env, oldpwd) != -1)
// 		env_add(oldpwd, data->env);
// 	else if (ft_strarr_str(data->env, oldpwd) == -1)
// 		replace_or_add_env(oldpwd, data);
// 	ft_memdel(oldpwd);
// 	return (0);
// }

// static int		go_to_path(int option, t_env *env)
// {
// 	int		ret;
// 	char	*path;

// 	path = NULL;
// 	if (option == 0)
// 	{
// 		update_oldpwd(env);
// 		path = get_env_path(env, "HOME", 4);
// 		if (!path)
// 			ft_printf_fd(STDERR_FILENO, "minishell : cd: HOME not set\n");
// 		if (!path)
// 			return (ERROR);
// 	}
// 	else if (option == 1)
// 	{
// 		path = get_env_path(env, "OLDPWD", 6);
// 		if (!path)
// 			ft_printf_fd(STDERR_FILENO, "minishell : cd: OLDPWD not set\n");
// 		if (!path)
// 			return (ERROR);
// 		update_oldpwd(env);
// 	}
// 	ret = chdir(path);
// 	ft_memdel(path);
// 	return (ret);
// }

// int	ft_cd(char **args, int args_len, t_global *data)
// {
// 	int		result;

// 	if (args_len > 2)
// 		ft_printf_fd(2, "minishell : cd: too many arguments");
// 	if (args_len == 1)
// 		return (go_to_path(0, data));
// 	if (ft_strcmp(args[1], "-") == 0)
// 		result = go_to_path(1, data);
// 	else
// 	{
// 		update_oldpwd(data);
// 		result = chdir(args[1]);
// 		if (result < 0)
// 			result *= -1;
// 		if (result != 0)
// 			print_error(args);
// 	}
// 	return (result);
// }
//cd "" doesnt change to home
//