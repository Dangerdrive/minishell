/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:54:13 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/11 18:53:54 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TABLE_SIZE 50

//for readline
# include <readline/readline.h>
# include <readline/history.h>

//
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

//my libs
# include "../libs/libft/libft.h"
# include "../libs/ft_printf/ft_printf.h"

//colors
# define RED     		"\033[1;31m"
# define GREEN   		"\033[1;32m"
# define YELLOW 		"\033[1;33m"
# define BLUE   	 	"\033[1;34m"
# define MAGENTA    	"\033[1;35m"
# define CYAN    		"\033[1;36m"
# define WHITE    		"\033[1;37m"
# define END   			"\033[0m"

typedef struct s_hash
{
	char	*content;
	char	*type;
	//t_hash	*next;
}	t_hash;

typedef struct s_global
{
	char		**env;
	t_hash		*hashtable[TABLE_SIZE];
	char		*usr_input;
	char		*cur_cwd_path;
	int			exit;
	//t_global	*next;
}	t_global;
#endif


t_global	*init_data(void);

/*---------------env----------------*/
int			copy_env(t_global **data);
void		free_env(t_global **data);

/*--------------token--------------*/
void		handle_input(t_global **data);
