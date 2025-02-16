/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute00.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:38:46 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:40:34 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_last_cmd_child(t_data *data, t_cmd *cmd_node, int input_fd);
static void	process_last_cmd(t_data *data, t_cmd *cmd_node, int input_fd);
static void	process_cmd(t_data *data, t_cmd *cmd_node,
				int input_fd, int *fd_pipe);

/*
The function `execute_cmds` refreshes the environment variables array based on the current linked list,  
and then calls the `recursive_pipeline` function to execute commands in a pipeline.  
It is used in the *Minishell* project to synchronize the state of environment variables  
and handle complex pipeline structures during the execution of commands.
*/
void	execute_cmds(t_data *data)
{
	if (data->envp_arr)
	{
		free_ft_split(data->envp_arr);
		data->envp_arr = NULL;
	}
	data->envp_arr = convert_envp_list_to_array(data->envp);
	recursive_pipeline(0, data, data->cmd);
}

/*
The function `recursive_pipeline` implements pipeline processing for commands in *Minishell*.  
It operates recursively, creating successive pipelines using the `pipe` function  
and child processes using `fork`. If `cmd_node` is the last in the pipeline,  
it calls the `process_last_cmd` function. Otherwise, it processes the current command  
in a child process using `process_cmd`, passing data through the pipeline to the next process.  
The parent process manages closing files and waits for the child processes to finish.  
This function enables the execution of a sequence of linked commands,  
where the output of one serves as the input for the next,  
which is a key feature in a UNIX-like shell.
*/
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

/*
The function `process_last_cmd_child` handles the execution of the last command in a pipeline  
in a child process in *Minishell*. It sets default signals, updates input and output file descriptors,  
duplicates file descriptors for the process's input and output, and closes unnecessary file descriptors.  
If the command is a built-in function, it calls it. If not, it searches for the executable file path  
in the environment variables and calls `execve` to execute the program with the provided environment.  
In case of an execution error, it displays a diagnostic message and terminates the process with the appropriate status.  
This function is crucial for correctly handling the last command in a pipeline,  
ensuring proper input/output management and error handling.
*/
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

/*
The function `process_last_cmd` in *Minishell* executes the last command in a pipeline,  
creating a new process using `fork`. In the child process, it calls the function `process_last_cmd_child`,  
which handles the command execution, manages input/output, and handles errors.  
The parent process ignores interrupt signals, closes unnecessary input file descriptors,  
waits for the child process to finish, and records the exit status of the command.  
This function is essential for handling the last command in a pipeline,  
ensuring proper process synchronization and setting the final exit status.
*/
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

/*
The function `process_cmd` in *Minishell* handles the execution of a single command within a pipeline.  
It sets the default signals, manages input/output through redirections and file descriptors,  
and closes unnecessary file descriptors in the pipeline.  
If the command is built-in, it is executed immediately.  
Otherwise, it searches for the full path to the program and runs it using `execve`.  
The function concludes when an error occurs or after the command has completed its execution.  
It is crucial for handling each step in a pipeline, ensuring correct data processing and process synchronization.
*/
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
