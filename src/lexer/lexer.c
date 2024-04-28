#include "../includes/minishell.h"

int	check_valid_input(t_tkn **node, int i)
{
	if (!input_starts_with_command((*node), i))
	{
		printf("%s: command not found\n", (*node)->content);
		return (0);
	}
	if ((is_double_special_token((*node)) && !(*node)->delimiter)
		|| (!(*node)->next && !ft_strcmp((*node)->type, SPECIAL_CHAR) && !(*node)->delimiter)
		|| is_and_or((*node)->content))
	{
		printf("Syntax error.\n");
		return (0);
	}
	return (1);
}

void	check_redirects(t_tkn **node)
{
	t_tkn	*head;

	head = *node;
	while (*node)
	{
		if ((*node)->content && is_redir((*node)->content)
			&& (*node)->next)
		{
			parse_redir(node, &head);
		}
		*node = (*node)->next;
	}
	*node = head;
}

int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!check_valid_input(&(*hashtable)[i], i))
			{
				(*hashtable)[i] = temp;
				return (0);
			}
			(*hashtable)[i] = (*hashtable)[i]->next;
		}
		(*hashtable)[i] = temp;
		remove_pipe(&(*hashtable)[i], i);
		check_redirects(&(*hashtable)[i]);
		i++;
	}
	return (1);
}



// TESTS
// ls > 123 | echo --> executa o redir, mas não pula linha no 'echo'
// cat 007 | wc --> (bug com arquivo q não existe) DÁ MENSAGEM DE ERRO MAS PRINTA '0	0	0' NA LINHA SEGUINTE
// export myvar=hello | echo $myvar --> era para pular uma linha apenas, mas tem 3 errors no valgrind.

// echo > 1 -n > 2 hahahahaha -- ok - mas no valgrind aparece uma mensagem de "invalid fd"
// ls | < 123 echo -- ok
// ls > qwe | < 123 -- ok
// ls | < 123 wc -- ok
// < 123 wc -- ok
// < 123
