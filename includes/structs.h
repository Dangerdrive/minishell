#ifndef STRUCTS_H
# define STRUCTS_H

# define TABLE_SIZE 50

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

typedef struct s_tkn
{
	char			*content;
	char			*type;
	char			*input;
	char			*output;
	char			*delimiter;
	char			*redir[TABLE_SIZE];
	t_bool			space_after;
	struct s_tkn	*prev;
	struct s_tkn	*next;
}	t_tkn;

typedef struct s_global
{
	char			**env;
	char			**exported;
	t_tkn			*hashtable[TABLE_SIZE];
	char			*usr;
	char			*usr_input;
	char			*cur_path;
	int				pid;
	int				is_exec;
	int				prev_process_status;
	int				ret;
	t_bool			is_echo;
	t_bool			exit;
	struct s_global	*next;
}	t_global;

#endif
