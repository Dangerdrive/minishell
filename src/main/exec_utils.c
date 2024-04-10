#include "../../includes/minishell.h"

char	**get_env_paths(t_global *data)
{
	char	*env_path_str;
	char	**paths;
	int		i;
	char	*tmp;

	i = 0;
	env_path_str = ft_getenv("PATH", &data);
	if (!env_path_str)
		return (NULL);
	paths = ft_split(env_path_str, ':');
	free(env_path_str);
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(tmp);
		if (!paths[i])
			return (NULL);
		i++;
	}
	return (paths);
}

char	*get_cmd_path(char*cmd, char**paths)
{
	int		i;
	char	*cmd_path;

	cmd_path = NULL;
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (!cmd_path)
		{
			ft_strarr_free(paths, ft_strarr_len(paths));
			ft_printf_fd(2, "cmd_path error\n");
			//cleanup_n_exit(ERROR, NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (NULL);
}

char	*get_cmd(char *cmd, t_global *data)
{
	char	**env_paths;
	char	*cmd_path;

	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	env_paths = get_env_paths(data);
	if (!env_paths)
		return (NULL);
	cmd_path = get_cmd_path(cmd, env_paths);
	if (!cmd_path)
	{
		ft_printf_fd(2, "%s: command not founds\n", cmd);
		ft_strarr_free(env_paths, ft_strarr_len(env_paths));
		return (NULL);
	}	
	ft_strarr_free(env_paths, ft_strarr_len(env_paths));
	return (cmd_path);
}

int	hashsize(t_tkn *hashtable)
{
	int		i;
	t_tkn	*temp;

	if (!hashtable)
		return (-1);
	temp = hashtable;
	i = 0;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char	**hash_to_args(t_tkn *hashtable)
{
	char	**args;
	t_tkn	*temp;
	int		i;
	int		args_count;

	args_count = hashsize(hashtable);
	args = malloc(sizeof(char *) * (args_count + 1));
	i = -1;
	temp = hashtable;
	while (temp)
	{
		args[++i] = ft_strdup(temp->content);
		temp = temp->next;
	}
	args[++i] = NULL;
	// //debug
	// while (i >= 0)
	// {
	// printf("%i: %s\n", i, args[i]);
	// 	i--;
	// }
	return (args);
}	