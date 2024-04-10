#include "../../includes/minishell.h"

void	replace_or_add_env(char *arg, t_global *data)
{
	char	*key;

	key = NULL;
	if (ft_strchr_i(arg, '=') != -1)
	{
		key = ft_strndup(arg, ft_strchr_i(arg, '='));
		ft_strarr_str_replace(data->env, key, arg);
	}
	else if (ft_strchr_i(arg, '=') == -1)
		key = ft_strdup(arg);
	if (ft_strarr_str(data->env, key) == -1
		&& ft_strarr_str(data->env, key) == -1)
		ft_strarr_stradd(&data->env, arg);
	if (key != NULL)
		free(key);
}

static int	update_oldpwd(t_global *data)
{
	char	cwd[4096];
	char	*oldpwd;

	if (getcwd(cwd, 4096) == NULL)
		return (1);
	oldpwd = ft_strjoin("OLDPWD=", cwd);
	if (!oldpwd)
		return (1);
	replace_or_add_env(oldpwd, data);
	ft_memdel(oldpwd);
	return (0);
}

// update oldpwd acontece antes do ft_getenv
static int	go_oldpwd(t_global *data)
{
	int		result;
	char	*path;

	path = NULL;
	ft_printf("OLDPWD: %s\n", ft_getenv("OLDPWD", &data));
	path = ft_getenv("OLDPWD", &data);
	if (!path)
	{
		ft_printf_fd(STDERR_FILENO, "minishell : cd: OLDPWD not set\n");
		return (0);
	}
	ft_printf("OLDPWD after: %s\n", ft_getenv("OLDPWD", &data));
	ft_printf("path: %s\n", path);
	result = chdir(path);
	update_oldpwd(data);
	//ft_memdel(path);
	return (result);
}

static int	go_home(t_global *data)
{
	int		result;
	char	*path;

	path = NULL;
	update_oldpwd(data);
	path = ft_getenv("HOME", &data);
	if (!path)
		ft_printf_fd(STDERR_FILENO, "minishell: cd: HOME not set\n");
	if (!path)
		return (0);
	result = chdir(path);
	//ft_memdel(path);
	return (result);
}

int	ft_cd(char **args, int args_len, t_global *data)
{
	int		result;

	if (args_len > 2)
	{
		result = 1;
		ft_printf_fd(2, "minishell : cd: too many arguments");
	}	
	else if (args_len == 1)
		result = go_home(data);
	else if (ft_strcmp(args[1], "-") == 0)
	{
		//problema na atualização do OLDPWD
		result = go_oldpwd(data);
	}
	else
	{
		update_oldpwd(data);
		result = chdir(args[1]);
		if (result < 0)
			result *= -1;
		if (result != 0)
			ft_printf_fd(2, "minishell: cd: %s: %s\n", args[1], strerror(errno));
	}
	return (result);
}
//cd "" doesnt change to home
