#ifndef STRUCTS_H
# define STRUCTS_H

# define TABLE_SIZE 50

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}	t_bool;

typedef enum e_sig
{
	PROMPT,
	EXEC,
	HEREDOC
}	t_sig;

typedef struct s_tkn
{
	char			*content;
	char			*type;
	char			*input;
	char			*output;
	int				input_fd;
	int				output_fd;
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
	t_sig			sig_env;
	char			*input;
	char			*output;
	int				original_stdin;
	int				original_stdout;
	int				original_fds[2];
	int				input_fd;
	int				output_fd;
	int				open_redirs;
	int				pipe[2];
	pid_t			pid;
	int				prev_process_status;
	int				ret;
	t_bool			exit;
	struct s_global	*next;
}	t_global;

#endif
