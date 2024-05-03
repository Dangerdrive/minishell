/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 21:55:37 by fde-alen          #+#    #+#             */
/*   Updated: 2024/05/02 21:55:43 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

# define SIMPLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define PATH_MAX 4096
# define IN 0
# define OUT 1

/*----------special-tokens---------*/
# define PIPE "|"
# define AMPERSAND "&"
# define LESS_THAN "<"
# define GREATER_THAN ">"
# define DOUBLE_LESS_THAN "<<"
# define DOUBLE_GREATER_THAN ">>"

/*-----------token-types-----------*/
# define COMMAND "command"
# define ARGUMENT "argument"
# define VARIABLE "variable"
# define EXPT_VARIABLE "export variable"
# define SPECIAL_CHAR "special char"
# define STRING_STRONG "string strong"
# define STRING_STD "string standard"

/*--------------colors-------------*/
# define RED     		"\033[1;31m"
# define GREEN   		"\033[1;32m" // \e[1;32m
# define YELLOW 		"\033[1;33m" // \e[1;33m
# define BLUE   	 	"\033[1;34m" // \e[1;34m
# define MAGENTA    	"\033[1;35m" // \e[1;35m
# define CYAN    		"\033[1;36m"
# define WHITE    		"\033[1;37m"
# define END   			"\033[0m"

/*--------------errors-------------*/
# define INTERRUPT 128
# define CMD_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define NOT_EXECUTABLE 126
# define OUT_OF_RANGE 255
# define BUILTIN_MISUSE 2
# define FORK_ERROR -1

#endif
