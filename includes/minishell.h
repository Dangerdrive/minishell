#ifndef MINISHELL_H
# define MINISHELL_H

//for readline
# include <readline/readline.h>
# include <readline/history.h>

//
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <errno.h>
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
# include "./structs.h"
# include "./macros.h"

t_global	*init_data(void);
int			handle_signal(t_global **data);
void		clean_stuff(t_global **data);
void		clean_input_and_hashtable(t_global **data);
char		*print_cwd(t_global **data);

/*------------BUILTINS-------------*/
int			exec_builtin(char **args, int args_len, t_global *data);
t_bool		is_builtin(char *command);

/*---------------env----------------*/
int			init_env(t_global **data);
t_bool		identifier_is_valid(char *str);
void		replace_or_add(char *arg, t_global *data);
int			ft_export(char **args, int args_len, t_global *data);
int			ft_unset(char **args, int args_len, t_global *data);
char		*ft_getenv(char *name, t_global **data);
int			ft_env(char **args, int args_len, t_global **data);

/*--------------echo----------------*/
int			ft_cd(char **args, int args_len, t_global *data);
int			ft_echo(char **args, int args_len);
int			ft_pwd(void);
int			ft_exit(char **args, int args_len, t_global *data);

/*--------------expand--------------*/
int			expand(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data);
char	    *search_value(t_global **data, char **str, int i, int len);
void		add_node_before(t_tkn **node, int i);
t_tkn		*add_node_after(t_tkn **node, int i);
t_bool		is_var_name(t_tkn **node, int i, int len);
t_bool		is_special_variable(char *var);
t_bool		is_special_var_char(char c);

/*--------------token--------------*/
int			readline_and_handle_input(t_global **data);
int			handle_input(t_global **data);
void		populate_hashtable(t_global **data, int idx, int len);
void		init_hashtable(t_tkn *(*hashtable)[TABLE_SIZE]);
t_tkn		*add_node(t_tkn **tkn_node, char **content);
int			check_exit_input(char **input, t_global *data);

/*--------------parse--------------*/
int			parse(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data);
t_bool  	is_export_var(char *content);
t_bool		is_pipe(char *token);
t_bool		is_and_or(char *token);
t_bool		is_special_token(char *token);
t_bool		is_double_special_token(t_tkn *node);

/*--------------lexer--------------*/
int			lexer(t_tkn	*(*hashtable)[TABLE_SIZE]);
t_bool		is_redir_in(char *c);
t_bool		is_redir_out(char *c);
t_bool		is_heredoc(char *content);
t_bool		is_redir(char *sig);

/*--------------export-------------*/
int			validate_identifier(char *str);

/*--------------lexer--------------*/
void		handle_signals(t_global **data);
void		interrupt_handler(int signal);

/*-------------signals-------------*/
void		define_prompt_signals(void);
void		define_exec_signals(void);
void		define_heredoc_signals(int pid);

/*--------------exec---------------*/
int			handle_execution(t_global *data);
int			hashsize(t_tkn *hashtable);
char		**hash_to_args(t_tkn *node);
char		*get_cmd(char *cmd, t_global *data);
char		*get_cmd_path(char *cmd, char **paths);
int			exec_one_process(t_global *data);

/*------------redirections---------*/
void		redirect_heredoc(t_global *data, int heredoc_number, char *eof);
int			redirect_input(char *input_redirect);
int			redirect_output(char *output_redirect);
void		expand_heredoc(t_global *data, char **line);
void		redirect_fd(int fd_to_redirect, int fd_location);
void		redirect_fds(int fd_in, int fd_out);
void		close_all_fds(void);

void		restore_original_fds(int original_fds[2]);
int			handle_redirects(t_global *data, int ori_fds[2]);

#endif
