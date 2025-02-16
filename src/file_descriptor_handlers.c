/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_descriptor_handlers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:43:12 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:44:41 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function `update_input_fd` handles and updates the input file descriptor for a command,  
checking various cases: if `here_doc` is set, it calls `get_heredoc` to create a pipe for input;  
if a redirection error occurs, it returns -1 as an error signal; if an input file is provided and there is no redirection error,  
it opens the file in read mode, updating the descriptor.  
This function ensures the correct input source for commands in a pipeline, handling files and here-docs.
*/
int	update_input_fd(t_cmd *cmd, int input_fd)
{
	if (cmd->here_doc)
	{
		input_fd = get_heredoc(cmd);
		if (input_fd < 0)
			ft_error_message(HEREDOC_ERR, -1);
		return (input_fd);
	}
	else if (cmd->redir_error == true)
		return (-1);
	else if (cmd->infile && cmd->redir_error == false)
	{
		input_fd = open(cmd->infile, O_RDONLY);
		if (input_fd < 0)
			perror(cmd->infile);
		return (input_fd);
	}
	return (input_fd);
}

/*
The function `get_output_fd` sets the output file descriptor for a given command.  
If the command has an assigned output file, it opens it in overwrite or append mode depending on the `append` flag.  
If there is no output file and the command is the last one, it uses standard output. Otherwise, it uses the write end of the pipe.  
This ensures that each command in the pipeline has an appropriately defined output target,  
and in the case of a file opening error, it generates a diagnostic message.
*/
int	get_output_fd(t_cmd *cmd, int *fd_pipe)
{
	int	output_fd;

	if (cmd->outfile && cmd->append == false)
		output_fd = open(cmd->outfile,
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else if (cmd->outfile && cmd->append == true)
		output_fd = open(cmd->outfile,
				O_WRONLY | O_CREAT | O_APPEND, 0664);
	else if (cmd->next == NULL)
		output_fd = STDOUT_FILENO;
	else
		output_fd = fd_pipe[1];
	if (output_fd < 0 && cmd->outfile)
		perror(cmd->outfile);
	else if (output_fd < 0 && !cmd->outfile)
		perror("output_fd");
	return (output_fd);
}

/*
The function `env_bltin` handles the built-in `env` command, which prints all the environment variables stored in the `data->envp` list.  
If additional arguments are provided to the `env` command, the function displays the error message "env: Too many arguments" and terminates with an error code `1`.  
Otherwise, it iterates through the list of environment variables and prints each of them.  
The function finishes by returning an exit code `0`.  
This is required in *minishell* to ensure proper functioning and displaying of environment variables according to the specification.
*/
void	env_bltin(t_data *data)
{
	t_envp	*envp;

	if (data->cmd->cmd[1])
	{
		ft_error_message("env: Too many arguments", 2);
		exit(1);
	}
	envp = data->envp;
	while (envp)
	{
		printf("%s\n", envp->value);
		envp = envp->next;
	}
	exit(0);
}

/*
The function `get_heredoc` reads data in "here-document" mode from standard input, writing it to a pipe until it encounters a special delimiter (indicated by `eof`).  
It returns the pipe file descriptor, which will be used as input for the next command.  
This function is used when the user wants to pass dynamic input to a command, ending with a special marker (e.g., `EOF`).  
The purpose is to handle dynamic input in pipelines.
*/
int	get_heredoc(t_cmd *cmd)
{
	int		fd_pipe[2];
	char	*input;
	char	*eof;

	eof = cmd->infile;
	pipe(fd_pipe);
	while (1)
	{
		write(1, "> ", 2);
		input = get_next_line(STDIN_FILENO);
		if (!input)
		{
			printf("\nWarning: here_doc delimited by EOF signal\n");
			break ;
		}
		if (ft_strncmp(input, eof, ft_strlen(eof)) == 0
			&& input[ft_strlen(eof)] == '\n')
			break ;
		write(fd_pipe[1], input, ft_strlen(input));
		free(input);
	}
	free(input);
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}
