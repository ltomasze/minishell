/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:19:19 by mbany             #+#    #+#             */
/*   Updated: 2025/01/02 15:04:18 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

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

/* errors */
# define MANY_ARGS_ERR "Error: minishell does not accept arguments"
# define NO_ENVP_ERR "Error: no environment found"

/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */
#define MISS_QUOTE_ERR "Syntax error: missing quote"
#define MISS_CMD_ERR "Syntax error: missing command"
#define SANITATION_ERR "Error: line sanitation error"

/* Print a message describing the meaning of the value of errno.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void perror (const char *__s);

//libft
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int	ft_atoi(const char *nptr);
char	*ft_itoa(int n);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtrim(char const *s1, char const *set);
void	ft_putstr_fd(char *s, int fd);
int	ft_isspace(char c);
int	ft_isalnum(int c);
int	ft_isalpha(int c);

//envp
void	free_envp(t_envp *head);
t_envp	*fetch_envp_node(t_envp *head, char *key);
void increment_shlvl(t_envp *head);
t_envp *fetch_envp (char **envp);
//signals
void handle_sigint(int sig);
void	handle_signals(void);
//free
void	free_ft_split(char **split);
void	ft_free_commands(t_cmd **commands);
//check
int	check_for_unclosed_quotes(char *line);
int	check_line_if_empty(char *line);
int	check_syntax(char *line);
//check1
int	check_for_preceding_command(char *line, int i);
int	check_for_following_command(char *line, int i);
void	go_to_next_quote(char *line, int *i, bool go_back);
int	check_for_missing_command(char *line);
//error
void	msg_error(char *err);
int	ft_error_message(char *str, int num);
//sanitation
void	sanitaze_line(t_data *data);
//sanitation1
char	*process_str(char *str, char *str_final, int str_final_len);

#endif