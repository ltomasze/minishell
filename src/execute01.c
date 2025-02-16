/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute01.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:40:45 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:42:16 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../includes/minishell.h"

static void	set_status_and_msg_err(char *err, int code, int *status);

static int	check_for_dot_builtin(char *cmd, int *status);
/*
The function `find_correct_path` searches for the full path to execute a given command `cmd` based on the `PATH` environment variable.  
It splits the value of the `PATH` variable into separate directories,  
concatenates each with the command name, and checks if the resulting path points to an executable file.  
If such a path is found, it returns the full path;  
otherwise, it returns `NULL`.  
This function allows locating and executing commands found in directories defined in the `PATH` variable.
*/

static char	*find_correct_path(t_envp *envp, char *cmd)
{
	char	*final_envp_path;
	char	**envp_paths;
	char	*envp_path_part;
	int		i;

	if (!envp || cmd[0] == '.')
		return (NULL);
	envp_path_part = ft_strjoin("/", cmd);
	envp_paths = ft_split(envp->value + 5, ':');
	i = 0;
	final_envp_path = NULL;
	while (envp_paths[i] != NULL)
	{
		final_envp_path = ft_strjoin(envp_paths[i], envp_path_part);
		if (access(final_envp_path, X_OK) == 0)
			break ;
		free(final_envp_path);
		final_envp_path = NULL;
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	return (final_envp_path);
}

/*
The function `find_cmd_path` finds the full path to a command by first checking if it is a built-in command (using a built-in check),  
and then checking if the command is an executable file.  
If the command lacks the appropriate permissions, it sets the corresponding error code.  
It then searches the environment variables (looking for the `PATH` variable)  
to find the directory in which the command can be executed.  
If it fails to find the path or the command does not exist,  
it sets an error and returns `NULL`.  
This function is part of a project that mimics shell behavior,  
including error handling when executing commands.
*/
char	*find_cmd_path(t_envp *envp, char *cmd, int *status)
{
	char	*final_envp_path;

	if (check_for_dot_builtin(cmd, status))
		return (NULL);
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
		return (cmd);
	else if (access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
	{
		set_status_and_msg_err(NO_PERM_ERR, 126, status);
		free(cmd);
		return (NULL);
	}
	while (envp && ft_strncmp(envp->value, "PATH", 4) != 0)
		envp = envp->next;
	final_envp_path = find_correct_path(envp, cmd);
	if (final_envp_path == NULL)
	{
		set_status_and_msg_err(NO_CMD_ERR, 127, status);
		free(cmd);
		return (NULL);
	}
	free(cmd);
	return (final_envp_path);
}

/*
The function `set_exit_status` analyzes the process exit code stored in the `status` variable  
and sets the appropriate exit code in `cmd_exit_status`.  
If the process ended normally, it saves its exit code.  
In the case of termination by signal, it adds a value of 128 to the signal number.  
When the process was stopped or resumed, it also sets the appropriate code.  
This function allows for correct reflection and passing of process exit codes in the shell,  
which is crucial for error and signal handling in the project.
*/
void	set_exit_status(int *cmd_exit_status, int status)
{
	if (WIFEXITED(status))
		*cmd_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*cmd_exit_status = 128 + WTERMSIG(status);
	else if (WIFSTOPPED(status))
		*cmd_exit_status = 128 + WSTOPSIG(status);
	else if (WIFCONTINUED(status))
		*cmd_exit_status = 128;
	else
		*cmd_exit_status = -1;
}

/*
The function `check_for_dot_builtin` checks whether the given command is a special case `"."` or `".."`.  
If so, it sets the appropriate error code and message, frees the memory allocated for the command,  
and returns a value of `1`, indicating that the command should not be executed further.  
Otherwise, it returns `0`.  
This function prevents errors when the user tries to use invalid system commands `"."` or `".."`,  
which increases compatibility with shell behavior.
*/
static int	check_for_dot_builtin(char *cmd, int *status)
{
	if (ft_strncmp(cmd, ".", 2) == 0)
	{
		set_status_and_msg_err(NO_FNAME_ARG_ERR, 2, status);
		free(cmd);
		return (1);
	}
	else if (ft_strncmp(cmd, "..", 3) == 0)
	{
		set_status_and_msg_err(NO_CMD_ERR, 127, status);
		free(cmd);
		return (1);
	}
	return (0);
}

static void	set_status_and_msg_err(char *err, int code, int *status)
{
	*status = code;
	msg_error(err);
}
