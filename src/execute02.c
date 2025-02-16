/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute02.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:42:29 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:42:59 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
The function `duplicate_fds` redirects file descriptors,  
setting the given input file descriptor (`input_fd`) as the standard input (`STDIN_FILENO`)  
and the output file descriptor (`output_fd`) as the standard output (`STDOUT_FILENO`) using `dup2`.  
If the redirection fails, it prints an appropriate error message.  
It is used in the *Minishell* project to handle input and output redirection in processes,  
enabling stream redirection for commands in pipelines or file redirections.
*/
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
