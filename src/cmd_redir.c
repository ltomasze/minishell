/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:19:37 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:22:04 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_here_doc_cmd(t_cmd *current_cmd, char *str);
static void	ft_append_cmd(t_cmd *current_cmd, char *str);
static void	ft_redir_out_cmd(t_cmd *current_cmd, char *str);
static void	ft_redir_in_cmd(t_cmd *current_cmd, char *str);

/*
The `ft_set_redir` function sets the appropriate redirection in the current command based on the token type.  
If there was a previous redirection error in the command, it does nothing.  
It creates a copy of the text from the next token (the file path or the redirection symbol),  
and depending on the type of the current token (`T_IN_REDIR`, `T_OUT_REDIR`, `T_APPEND`, `T_HEREDOC`),  
it calls the appropriate function (`ft_redir_in_cmd`, `ft_redir_out_cmd`,  
`ft_append_cmd`, `ft_here_doc_cmd`) to set the redirection in the `t_cmd` structure.  
If there is a memory allocation issue, it returns an error.
*/
int	ft_set_redir(t_token **current_tok, t_cmd *current_cmd)
{
	char	*str;

	if (current_cmd->redir_error == true)
		return (0);
	str = ft_strdup((*current_tok)->next->text);
	if (!str)
		return (ft_perror_message());
	if ((*current_tok)->type == T_IN_REDIR)
		ft_redir_in_cmd(current_cmd, str);
	if ((*current_tok)->type == T_OUT_REDIR)
		ft_redir_out_cmd(current_cmd, str);
	if ((*current_tok)->type == T_APPEND)
		ft_append_cmd(current_cmd, str);
	if ((*current_tok)->type == T_HEREDOC)
		ft_here_doc_cmd(current_cmd, str);
	return (0);
}

/*
The `ft_redir_in_cmd` function handles setting the input file for the command in the `t_cmd` structure.  
It checks whether the file `str` is available for reading; if not,  
it sets the redirection error in the `redir_error` field,  
frees memory for `str`, and ends the operation.  
If access is valid, it frees the previous input file (if it exists),  
assigns the new file to `infile`, and sets `here_doc` to `false` to indicate no use of a heredoc.
*/
static void	ft_redir_in_cmd(t_cmd *current_cmd, char *str)
{
	if (ft_check_access(str, READ) < 0)
	{
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	if (current_cmd->infile)
		free(current_cmd->infile);
	current_cmd->infile = str;
	current_cmd->here_doc = false;
}

/*
The `ft_redir_out_cmd` function sets the output file for the command in the `t_cmd` structure.  
It tries to open the file `str` in write mode, creating it if it doesn't exist and clearing its contents if it does.  
If the file opening fails, it prints an error message, sets the redirection error flag `redir_error`, and frees the memory for `str`.  
If the opening succeeds, it closes the file descriptor, frees any previous output file, assigns the new one to `outfile`, 
and sets `append` to `false` to indicate that the data should be overwritten.
*/
static void	ft_redir_out_cmd(t_cmd *current_cmd, char *str)
{
	int	fd;

	fd = open(str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_perror_message();
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	close(fd);
	if (current_cmd->outfile)
		free(current_cmd->outfile);
	current_cmd->outfile = str;
	current_cmd->append = false;
}

/*
The `ft_append_cmd` function opens the file `str` in append mode (O_APPEND) 
or creates it if it doesn't exist (O_CREAT).  
If the opening fails, it sets the redirection 
error flag in the `current_cmd` structure, prints an error message, 
and frees the memory for `str`.  
Otherwise, it closes the file, 
updates the `outfile` pointer in `current_cmd` to the new file `str`, 
and sets the append mode (`append`) to `true`.
*/
static void	ft_append_cmd(t_cmd *current_cmd, char *str)
{
	int	fd;

	fd = open(str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_perror_message();
		current_cmd->redir_error = true;
		free(str);
		return ;
	}
	close(fd);
	if (current_cmd->outfile)
		free(current_cmd->outfile);
	current_cmd->outfile = str;
	current_cmd->append = true;
}

/*
The `ft_here_doc_cmd` function sets the input file in the `current_cmd` structure to `str`,  
freeing the memory previously occupied by the old input file if it existed, and marks  
that a here document command is being used by setting the `here_doc` flag to `true`.
*/
static void	ft_here_doc_cmd(t_cmd *current_cmd,
char *str)
{
	if (current_cmd->infile)
		free(current_cmd->infile);
	current_cmd->infile = str;
	current_cmd->here_doc = true;
}
