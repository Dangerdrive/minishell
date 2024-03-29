/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:54:13 by gde-souz          #+#    #+#             */
/*   Updated: 2024/03/12 16:51:48 by gde-souz         ###   ########.fr       */
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

typedef struct s_tkn
{
	char			*content;
	char			*type;
	struct s_tkn	*prev;
	struct s_tkn	*next;
}	t_tkn;

typedef struct s_global
{
	char			**env;
	t_tkn			*hashtable[TABLE_SIZE];
	char			*usr_input;
	char			*cur_cwd_path;
	int				exit;
	struct s_global	*next;
}	t_global;

t_global	*init_data(void);
void		handle_signal(void);
void		free_stuff(t_global *data);

/*---------------env----------------*/
int			copy_env(t_global **data);
void		free_env(t_global **data);

/*--------------token--------------*/
void		handle_input(t_global **data);

#endif
