/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:59:03 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 19:38:13 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	set_status_and_msg_err(char *err, int code, int *status)
{
	*status = code;
	msg_error(err);
}

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