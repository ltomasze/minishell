/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:47:58 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 19:32:24 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_last_cmd_child(t_data *data, t_cmd *cmd_node, int input_fd)
{
	int	output_fd;
	int	status;

	set_signals_to_default();
	input_fd = update_input_fd(cmd_node, input_fd);
	output_fd = get_output_fd(cmd_node, NULL);
	duplicate_fds(input_fd, output_fd);
	if (input_fd > 0)
		close(input_fd);
	if (output_fd > 2)
		close(output_fd);
	if (!cmd_node->cmd)
		exit(0);
	check_for_builtin_and_execute(cmd_node->cmd, data);
	cmd_node->cmd[0] = find_cmd_path(data->envp, cmd_node->cmd[0], &status);
	if (cmd_node->cmd[0] && output_fd > 0 && cmd_node->redir_error == false)
		status = execve(cmd_node->cmd[0], cmd_node->cmd, data->envp_arr);
	if (status < 0)
		perror("execve failed");
	exit(status);
}

static void	process_cmd(t_data *data, t_cmd *cmd_node,
	int input_fd, int *fd_pipe)
{
	int	output_fd;
	int	status;

	set_signals_to_default();
	input_fd = update_input_fd(cmd_node, input_fd);
	if (input_fd < 0)
	{
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		exit(1);
	}
	output_fd = get_output_fd(cmd_node, fd_pipe);
	duplicate_fds(input_fd, output_fd);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	if (input_fd > 0)
		close(input_fd);
	if (!cmd_node->cmd)
		exit(0);
	check_for_builtin_and_execute(cmd_node->cmd, data);
	cmd_node->cmd[0] = find_cmd_path(data->envp, cmd_node->cmd[0], &status);
	if (cmd_node->cmd[0] && input_fd >= 0 && cmd_node->redir_error == false)
		status = execve(cmd_node->cmd[0], cmd_node->cmd, data->envp_arr);
	exit(status);
}

static void	process_last_cmd(t_data *data, t_cmd *cmd_node, int input_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		perror("fork failed");
	else if (pid == 0)
		process_last_cmd_child(data, cmd_node, input_fd);
	else
	{
		signal(SIGINT, SIG_IGN);
		if (input_fd > 0)
			close(input_fd);
		waitpid(pid, &status, 0);
		set_exit_status(&(data->cmd_exit_status), status);
	}
}

void	recursive_pipeline(int input_fd, t_data *data, t_cmd *cmd_node)
{
	int		fd_pipe[2];
	int		status;
	pid_t	pid;

	if (cmd_node->next == NULL)
		process_last_cmd(data, cmd_node, input_fd);
	else
	{
		pipe(fd_pipe);
		pid = fork();
		if (pid < 0)
			perror("fork failed");
		else if (pid == 0)
			process_cmd(data, cmd_node, input_fd, fd_pipe);
		else
		{
			signal(SIGINT, SIG_IGN);
			close(fd_pipe[1]);
			if (input_fd > 0)
				close(input_fd);
			recursive_pipeline(fd_pipe[0], data, cmd_node->next);
			waitpid(pid, &status, 0);
		}
	}
}

void	execute_cmds(t_data *data)
{
	if (data->envp_arr)
	{
		free_ft_split(data->envp_arr);
		data->envp_arr = NULL;
	}
	data->envp_arr = convert_envp_llist_to_array(data->envp);
	recursive_pipeline(0, data, data->cmd);
}