/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:22:36 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:30:05 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the input at a specific index starts with a valid command.
 *
 * @param[in] node Pointer to the token node being checked.
 * @param[in] i Index in the token array.
 * @return 1 if valid command found, 0 otherwise, with error message output.
 */
int	check_valid_input(t_tkn **node)
{
	if ((is_double_special_token((*node)) && !(*node)->delimiter)
		|| (!(*node)->next && !ft_strcmp((*node)->type, SPECIAL_CHAR)
			&& !(*node)->delimiter) || is_and_or((*node)->content))
	{
		ft_dprintf(2, "Syntax error.\n");
		return (0);
	}
	return (1);
}

/**
 * Traverses through token nodes and processes redirections.
 *
 * @param[in, out] node Pointer to the current token node to check.
 */
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

/**
 * Performs lexical analysis on a hashtable of tokens, checking commands
 * and processing redirections.
 *
 * @param[in, out] hashtable Pointer to the hashtable of token nodes.
 * @return 1 on successful analysis, 0 on finding an error.
 */
int	lexer(t_tkn	*(*hashtable)[TABLE_SIZE])
{
	int		i;
	t_tkn	*temp;

	i = 0;
	while ((*hashtable)[i])
	{
		temp = (*hashtable)[i];
		while ((*hashtable)[i])
		{
			if (!check_valid_input(&(*hashtable)[i]))
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
// cat 007 | wc --> (bug com arquivo q não existe)
// DÁ MENSAGEM DE ERRO MAS PRINTA '0	0	0' NA LINHA SEGUINTE
// export myvar=hello | echo $myvar -->
//era para pular uma linha apenas, mas tem 3 errors no valgrind.

// echo > 1 -n > 2 hahahahaha -- ok -
//mas no valgrind aparece uma mensagem de "invalid fd"
// ls | < 123 echo -- ok
// ls > qwe | < 123 -- ok
// ls | < 123 wc -- ok
// < 123 wc -- ok
// < 123

// ECHO:
//in a case like echo $USER$USER, the result have a space in between
//export test test1=123 && echo $test
//results in 123. Expansion is being ambiguos.

//EXPORT:
// 	//primeiro e segundo e 4º devem ser adicionados.
// 	//USER deve modificar o valor de USER
// mas não deve modificar o valor de USER_ZDOTDIR
// 	//123test deve ser invalido (começa com numero)
// 	//PATH não deve ser alterado, pois não tem igual.
// 	//XMODIFIERS deve ser modificado para valor vazio
