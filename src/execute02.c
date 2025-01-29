/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute02.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:34:09 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:22:18 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `duplicate_fds` przekierowuje deskryptory plików, 
ustawiając podany deskryptor wejścia (`input_fd`) 
jako standardowe wejście (`STDIN_FILENO`) 
i deskryptor wyjścia (`output_fd`) jako standardowe wyjście 
(`STDOUT_FILENO`) za pomocą `dup2`. 
Jeśli przekierowanie się nie powiedzie, 
wypisuje odpowiedni komunikat błędu. 
Jest wykorzystywana w projekcie *Minishell* 
do obsługi redirekcji wejścia i wyjścia w procesach, 
umożliwiając przekierowanie strumieni dla poleceń 
w pipeline lub redirekcji plikowej.
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
