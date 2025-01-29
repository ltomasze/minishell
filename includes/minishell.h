/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:19:19 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:42:37 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

typedef struct s_envp
{
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_cmd
{
	char			**cmd;
	char			*infile;
	char			*outfile;
	bool			append;
	bool			here_doc;
	bool			redir_error;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	t_cmd		*cmd;
	t_envp		*envp;
	char		**envp_arr;
	char		*line;
	int			cmd_exit_status;
}	t_data;

typedef struct s_token
{
	int				type;
	char			*text;
	struct s_token	*next;
}	t_token;

//tokens
# define T_OUT_REDIR	1 
# define T_IN_REDIR		2
# define T_APPEND		3
# define T_HEREDOC		4
# define T_PIPE			5
# define T_WORD			6
# define T_ARG			7

// errors
# define MANY_ARGS_ERR "Error: minishell doesn't accept arguments"
# define NO_ENVP_ERR "Error: no environment found"
# define MISS_QUOTE_ERR "Error: missing quote"
# define MISS_CMD_ERR "Error: missing command"
# define SANITIZE_ERROR "Error: line sanitize error"
# define REDIR_TO_OPR "Syntax error: unexpected token"
# define NULL_REDIR "Error: redirect got confused, it's not sure where to go!"
# define NUM_REQ_ERR "Exit error: We need a number, not a magic trick!"
# define NO_PERM_ERR "Error: You shall not pass. Permission denied"
# define NO_CMD_ERR "Error: check command"
# define NO_FNAME_ARG_ERR "Error: filename argument required"
# define HEREDOC_ERR "Error: heredoc malfunction"

// Standard file descriptors.
# define STDIN_FILENO	0	/* Standard input.  */
# define STDOUT_FILENO	1	/* Standard output.  */
# define STDERR_FILENO	2	/* Standard error output.  */

//permissions to file
# define READ			1
# define WRITE			2
# define EXECUTE		3

/* Print a message describing the meaning of the value of errno.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void	perror(const char *__s);

//libft
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strdup(const char *s);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
int			ft_atoi(const char *nptr);
char		*ft_itoa(int n);
char		*ft_strchr(const char *s, int c);
char		*ft_strtrim(char const *s1, char const *set);
void		ft_putstr_fd(char *s, int fd);
int			ft_isspace(char c);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		**ft_split(char const *s, char c);

//main
void		free_resources(t_data *data);

//envp
void		free_envp(t_envp *head);
t_envp		*fetch_envp_node(t_envp *head, char *key);
void		increment_shlvl(t_envp *head);
t_envp		*fetch_envp(char **envp);
int			append_envp_node(t_envp **head, char *str);
void		remove_envp_node(t_envp *prev_node);
t_envp		*fetch_node_before(t_envp **head, char *key);
char		**convert_envp_list_to_array(t_envp *head);

//signals
void		handle_sigint(int sig);
void		handle_signals(void);
void		set_signals_to_default(void);

//free
void		free_ft_split(char **split);
void		ft_free_commands(t_cmd **commands);

//check_syntax
int			check_syntax(char *line);
int			check_if_line_is_empty(char *line);
int			check_for_unclosed_quotes(char *line);
int			check_for_missing_command(char *line);

//error
int			ft_error_message(char *str, int num);
void		msg_error(char *err);
int			ft_perror_message(void);
int			ft_perror_free(char *first, char *second, char *third);

//utils
void		go_to_next_quote(char *line, int *i, bool go_back);
int			ft_check_access(char *file, int type);

//sanitize
void		sanitize_line(t_data *data);
char		*process_str(char *str, char *str_final,
				int str_final_len);

//commands
int			ft_set_command(t_cmd **commands);
int			ft_commands_creation(t_data *data);
t_cmd		*ft_commands(t_token *tokens);
int			ft_command(t_token **cur_token, t_token *tokens,
				t_cmd **cur_command, t_cmd *cmds);
int			ft_redir(t_token **current_tok, t_token *head_tok,
				t_cmd **current_cmd, t_cmd *head_cmd);
int			ft_pipe(t_token **current_token,
				t_token *tokens, t_cmd **current_cmd, t_cmd *commands);
// static int	ft_count_tok(t_token *tokens);

//cmd_redir.c
int			ft_set_redir(t_token **current_tok, t_cmd *current_cmd);

//tokens
t_token		*ft_tokenizer(t_data *data, char *inp);
void		ft_free_tokens(t_token **tokens);
int			ft_is_redir(char *input, int *i, t_token **tokens);
int			ft_is_pipe(char *input, int *i, t_token **tokens);
int			ft_is_word(char *input, int *i, t_token **tokens, t_data *data);
int			create_token(char *str, int type, t_token **tokens);

// static int	ft_single_redirection(char x, t_token **tokens, char *str);
int			ft_create_word_tok(char *str, int *i,
				t_token **tokens, t_data *data);
void		ft_skip_sq(int *n, char *str);
int			ft_extract_word(char *str, int *n, t_token **tokens, t_data *data);
int			ft_check_for_dollar(char **word, t_data *data);
int			ft_cross_word(char **word, t_token **tokens);
int			ft_cross_dq(int *i, char **word, t_data *data);
int			ft_clear_quote(int *i, char **word, char del);
int			ft_cut_token(int *i, char **word, t_token **tokens);
/*
static int	ft_append_redir(char *input, int *i, 
	t_token **tokens, char *str);
*/

//clean tokens
int			ft_check_tokens(t_token **tokens);
/*
static int	ft_remove_empty_tokens(t_token **tokens);
static void	ft_remove_token(t_token **head, 
	t_token **to_del, t_token **prev);
*/

//dollar
int			ft_dollar(int *i, char **word, t_data *data);
//static int	ft_valid_dollar(int *i, char *word, char **var);
//static int	ft_expand_var(char *var, t_envp *env, char **word, int *i)

//builtin.c
void		exit_bltin(t_data *data);

//builtin_export
int			export_bltin(char **cmd, t_data *data);
int			ft_print_env_var(t_data *data);

//builtin_pwd
void		pwd_bltin(void);

//builtin_unset
int			unset_bltin(char **cmd, t_data *data);
void		ft_remove_head_node(t_envp **head);

//builtin_cd
int			cd_bltin(char **cmd, t_data *data);
// static int	ft_cd_home(t_data *data, t_envp *home);
// static int	cd_handler(char *str, t_data *data);
int			check_for_builtin_and_execute(char **cmd, t_data *data);

//execute
void		execute_cmds(t_data *data);
void		recursive_pipeline(int input_fd, t_data *data, t_cmd *cmd_node);
/*
static void	process_last_cmd(t_data *data, t_cmd *cmd_node, int input_fd)
static void	process_last_cmd_child(t_data *data, t_cmd *cmd_node, int input_fd);
*/
char		*find_cmd_path(t_envp *envp, char *cmd, int *status);
void		set_exit_status(int *cmd_exit_status, int status);
//static int	check_for_dot_builtin(char *cmd, int *status);
//static void	set_status_and_msg_err(char *err, int code, int *status);
//static char	*find_correct_path(t_envp *envp, char *cmd);

// file_descriptor_handlers
int			update_input_fd(t_cmd *cmd, int input_fd);
int			get_output_fd(t_cmd *cmd, int *fd_pipe);
void		duplicate_fds(int input_fd, int output_fd);
void		env_bltin(t_data *data);
int			get_heredoc(t_cmd *cmd);

//builtin_echo
void		echo_bltin(char **cmd);

//buildin_echo
void		pwd_bltin(void);

// get next line
char		*get_next_line(int fd);
#endif