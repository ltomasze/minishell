/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 19:04:09 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 19:49:40 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	duplicate_fds(int input_fd, int output_fd)
{
	if (input_fd > 0)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
			perror("dup2 failed");
	}
	if (output_fd > 2)
	{
		if (dup2(output_fd, STDOUT_FILENO) < 0)
			perror("dup2 failed");
	}
}

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