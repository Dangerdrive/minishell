/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gde-souz <gde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:29:39 by gde-souz          #+#    #+#             */
/*   Updated: 2024/04/30 17:29:40 by gde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "./structs.h"
# include "./macros.h"

t_global	*init_data(void);
void		clean_stuff(t_global **data);
void		clean_input_and_hashtable(t_global **data);
void		free_redir_args(char *(*args)[TABLE_SIZE]);
int			handle_prompt(t_global **data);
char		*print_cwd(t_global **data);

/*------------BUILTINS-------------*/
int			exec_builtin(char **args, int args_len, t_global *data);
t_bool		is_builtin(char *command);
t_bool		unset_identifier_is_valid(char *str);
int			key_matches_target(const char *strarr_elem, const char *tgt);

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
char		*search_value(t_global **data, char **str, int i, int len);
void		add_node_before(t_tkn **node, int i);
t_tkn		*add_node_after(t_tkn **node, int i);
t_bool		is_var_name(t_tkn **node, int i, int len);
t_bool		is_special_variable(char *var, t_bool print);
t_bool		is_special_var_char(char c);

/*--------------token--------------*/
int			tokenize(t_global **data);
void		populate_hashtable(t_global **data, int idx, int len);
void		init_hashtable(t_tkn *(*hashtable)[TABLE_SIZE]);
t_tkn		*add_node(t_tkn **tkn_node, char **content);
int			check_exit_input(char **input, t_global *data);

/*--------------parse--------------*/
int			parse(t_tkn *(*hashtable)[TABLE_SIZE], t_global **data);
t_bool		is_export_var(char *content);
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
//t_bool		input_starts_with_command(t_tkn	*node, int i);
void		check_redirects(t_tkn **node);
void		remove_pipe(t_tkn **node, int i);
void		parse_redir(t_tkn **node, t_tkn **temp_node);
void		check_heredoc(t_tkn **node);

/*-------------signals-------------*/
void		define_prompt_signals(void);
void		define_exec_signals(int pid);
void		define_heredoc_signals(int pid);

/*--------------exec---------------*/
int			handle_execution(t_global *data);
int			hashsize(t_tkn *hashtable);
char		**hash_to_args(t_tkn *node);
char		*get_cmd(char *cmd, t_global *data);
char		*get_cmd_path(char *cmd, char **paths);
int			exec_one_process(t_global *data);
int			exec_processes(t_global *data);
int			exec_nonbuiltin(char **args, t_global *data);
int			pipecount(t_global *data);
void		handle_pipe(int original_fd_out, t_global *data,
				t_tkn *current_node, t_tkn **hashtable);
int			wait_for_child(int child_pid, int is_last_child);
int			handle_signal_interrupt(int status, int is_last_child);
int			is_folder(char *command);
void		external_exit(int exit_status);

/*------------redirections---------*/
int			handle_redirects(t_global *data, int ori_fds[2]);
// void		handle_redirects_for_pipes(t_global *data,
// 				char *(*redirects)[TABLE_SIZE]);
void		handle_redirects_for_pipes(char *(*redirects)[TABLE_SIZE]);
void		redirect_heredoc(t_global *data, int heredoc_number, char *eof);
int			redirect_input(char *input_redirect, int i);
int			redirect_output(char *output_redirect);
void		expand_heredoc(t_global *data, char **line);
void		redirect_fd(int fd_to_redirect, int fd_location);
void		redirect_fds(int fd_in, int fd_out);
void		close_all_fds(void);
void		close_extra_fds(void);
void		restore_original_fds(int original_fds[2]);
void		restore_fds(int original_fds[2]);
//int			handle_redirects(t_global *data, int ori_fds[2]);
void		save_original_fd_in(int original_fds[2]);
void		save_original_fd_out(int original_fds[2]);
int			handle_input_redirect(char *redir, int here_num, int original_fds[2]);
int			handle_output_redirect(char *command, int original_fds[2]);
char		*tmp_filename(int heredoc_number);
t_bool		ends_with_linebreak(char *line);
int			get_fd(char *filename);
int			get_var_value1(t_global **data, char **line, int i);
#endif
