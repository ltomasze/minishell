/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:48:07 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:56:21 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*The sig parameter represents the signal number that was sent to the program.
It is not used in this function, so it is "ignored" (i.e., not processed).
This is a common practice when the signal is handled generally,
without the need to consider the specifics of the signal.
write(STDOUT_FILENO, "\n", 1);  // Write a new line to STDOUT
This aims to improve the appearance of the terminal so that after pressing Ctrl+C
(which sends the SIGINT signal), the cursor moves to a new line,
and the user doesn't see a cluttered line.
rl_replace_line("", 0);  // Remove the current line from the readline buffer
This function removes the current line from the Readline buffer.
In this case, the line is replaced with an empty string (i.e., the line is deleted).
rl_on_new_line();  // Switch to a new line in the readline buffer
rl_redisplay();  // Redisplay the line in the readline buffer
This ensures that the user sees an empty line, rather than the line they just cleared.
In summary, the handle_sigint function ensures that after pressing Ctrl+C:
The cursor moves to a new line.
The previous line is removed from the buffer.
The terminal remains clean (with a new empty line), instead of being in a "dirty" state. */
void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/*
signal(SIGINT, handle_sigint); 
This is used so that after pressing Ctrl+C, the program doesn’t exit to the default shell.
It customizes the handling of the SIGINT signal (sent when Ctrl+C is pressed) by calling the handle_sigint function.
signal(SIGQUIT, SIG_IGN);  
This ensures that when pressing Ctrl+\ (which sends SIGQUIT), it does not interrupt the program.
The signal is ignored, meaning it doesn't terminate the program with a core dump.*/


void	handle_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/*
The function set_signals_to_default restores the default behavior for the signals SIGQUIT (exit from the program) and SIGINT (interrupting the process, e.g., when pressing Ctrl+C).
It is used in the Minishell project to revert the system’s standard handling for these signals, which can be modified during the shell’s execution, such as allowing the shell to correctly respond to interruptions.
*/
void	set_signals_to_default(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}
