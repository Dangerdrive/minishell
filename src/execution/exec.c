#include "../../includes/minishell.h"

int	handle_execution(t_global *data)
{
	//int		ret;
	char	**args;

	args = NULL;
	if (data->hashtable[0]->content)
		args = hash_to_args(data->hashtable[0]);
	//ret = 1;
	if (pipecount(data) == 0)
		data->ret = exec_one_process(data);
	else if (pipecount(data) > 0)
		data->ret = exec_processes(data);
	if (data->hashtable[0]->content)
		ft_strarr_free(args, ft_strarr_len(args));
	return (data->ret);
}
