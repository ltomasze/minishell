/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 14:46:27 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 14:52:10 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
Funkcja `pwd_bltin` realizuje działanie wbudowanej komendy `pwd`,
 która wypisuje aktualną ścieżkę roboczą. Używa funkcji `getcwd` 
 do pobrania bieżącej ścieżki, a jeśli wystąpi błąd, wywołuje funkcję 
 `ft_perror_message` i kończy działanie z kodem błędu `1`. 
 Jeśli ścieżka zostanie pobrana pomyślnie, wypisuje ją na standardowe wyjście. 
 Funkcja kończy działanie z kodem `0`. Jest częścią projektu *minishell*, 
 odwzorowując polecenie `pwd` w interpreterze komend.
*/
void	pwd_bltin(void)
{
	char	path[4096];

	if (!(getcwd(path, 4096)))
	{
		ft_perror_message();
		exit(1);
	}
	else
		printf("%s\n", path);
	exit(0);
}
