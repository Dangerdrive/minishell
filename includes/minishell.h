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
# define FILE_TXT "file"
# define VARIABLE "variable"
# define SPECIAL_CHAR "special char"
# define STRING_STRONG "string strong"
# define STRING_STD "string standard"

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

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

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
	char			**exported;
	t_tkn			*hashtable[TABLE_SIZE];
	char			*usr_input;
	char			*cur_path;
	int				ret;
	int				exit;
	struct s_global	*next;
}	t_global;

t_global	*init_data(void);
int			handle_signal(void);
void		clean_stuff(t_global **data);
void		clean_input_and_hashtable(t_global **data);

/*------------BUILTINS-------------*/
int			exec_builtin(char **args, int args_len, t_global *data);
t_bool		is_builtin(char *command);
/*---------------env----------------*/
int			init_env(t_global **data);
t_bool		identifier_is_valid(char *str);
void		replace_or_add(char *arg, t_global *data);
int			ft_export(char **args, t_global *data);
int			ft_unset(char **args, int args_len, t_global *data);
int			ft_env(t_global **data);
/*--------------echo----------------*/
int			ft_echo(char **args, int args_len);
int			ft_pwd(void);

/*---------------exp----------------*/
void		expand(t_tkn *(*hashtable)[TABLE_SIZE], char **env);

/*--------------token--------------*/
int			readline_and_handle_input(t_global **data);
int			handle_input(t_global **data);
void		populate_hashtable(t_global **data, int idx, int len);
void		init_hashtable(t_tkn *(*hashtable)[TABLE_SIZE]);
void		add_node(t_tkn **tkn_node, char **content);
int			check_exit_input(char **input, int *exit);

/*--------------parse--------------*/
int			parse(t_tkn *(*hashtable)[TABLE_SIZE], char **env);
t_bool		is_pipe(char *token);
t_bool		is_and_or(char *token);
t_bool		is_pipe_and_or(char *token);
t_bool		is_special_token(char *token);

/*--------------lexer--------------*/
int			lexer(t_tkn	*(*hashtable)[TABLE_SIZE]);

#endif
