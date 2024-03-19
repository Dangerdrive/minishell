#ifndef MINISHELL_H
# define MINISHELL_H

# define TABLE_SIZE 50

# define SIMPLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define PIPE "|"
# define LOGIC_OR "||"
# define LOGIC_AND "&&"
# define AMPERSAND "&"
# define LESS_THAN "<"
# define GREATER_THAN ">"
# define DOUBLE_LESS_THAN "<<"
# define DOUBLE_GREATER_THAN ">>"
# define COMMAND "command"
# define FLAG "flag"
# define SPECIAL_CHAR "special char"
# define STRING_STRONG "string strong"
# define STRING_STD "double standard"

//colors
# define RED     		"\033[1;31m"
# define GREEN   		"\033[1;32m"
# define YELLOW 		"\033[1;33m"
# define BLUE   	 	"\033[1;34m"
# define MAGENTA    	"\033[1;35m"
# define CYAN    		"\033[1;36m"
# define WHITE    		"\033[1;37m"
# define END   			"\033[0m"

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
	char			*cur_path;
	int				exit;
	struct s_global	*next;
}	t_global;

t_global	*init_data(void);
void		handle_signal(t_global **data);
void		clean_stuff(t_global **data);
void		clean_input_and_hashtable(t_global **data);

/*---------------env----------------*/
int			copy_env(t_global **data);
void		free_env(t_global **data);

/*--------------token--------------*/
int			readline_and_handle_input(t_global **data);
int			handle_input(t_global **data);
void		populate_hashtable(t_global **data, int i, int len);
void		init_hashtable(t_tkn *(*hashtable)[TABLE_SIZE]);
void		add_node(t_tkn **tkn_node, char *input, int i, int len);
int			check_exit_input(char **input, int *exit);

/*--------------parse--------------*/
void		parse(t_tkn *(*hashtable)[TABLE_SIZE]);

#endif
