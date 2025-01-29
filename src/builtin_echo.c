/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 14:33:45 by mbany             #+#    #+#             */
/*   Updated: 2025/01/23 19:50:55 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_echo_args(char **cmd);

/*
Funkcja `echo_bltin` realizuje działanie wbudowanej komendy `echo`, 
wypisując argumenty przekazane w `cmd` na standardowe wyjście. 
Obsługuje flagę `-n`, która pomija dodanie znaku nowej linii na końcu. 
Analizuje kolejne argumenty, sprawdzając poprawność flagi `-n`. 
Jeśli flaga występuje poprawnie, ustawia zmienną `flag` na `true` 
i pomija dodawanie nowej linii po wypisaniu argumentów. 
Po przetworzeniu funkcja wywołuje `ft_echo_args` 
do wypisania właściwych argumentów i kończy proces kodem `0`. 
Funkcja służy do odwzorowania działania polecenia `echo` w *minishell*.
*/
void	echo_bltin(char **cmd)
{
	int		i;
	int		j;
	bool	flag;

	i = 1;
	j = 0;
	flag = false;
	while (cmd[i])
	{
		if (!cmd[i][j] || cmd[i][j] != '-')
			break ;
		j++;
		while (cmd[i][j] && cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] || j == 1)
			break ;
		j = 0;
		flag = true;
		i++;
	}
	ft_echo_args(&cmd[i]);
	if (flag == false)
		printf("\n");
	exit(0);
}

/*
Funkcja `ft_echo_args` iteruje przez argumenty przekazane 
w tablicy `cmd` (zawierającej komendy i ich argumenty), 
wypisuje każdy z nich na standardowe wyjście, 
dodając spację między argumentami, ale nie na końcu, 
a następnie kończy działanie. Jest to implementacja w projekcie, 
która wykonuje podobną funkcję do standardowej komendy `echo`, 
wypisując argumenty komendy na ekran, 
ale nie dodaje domyślnie nowej linii. 
Funkcja jest używana, aby wyświetlić wszystkie argumenty bez dodatkowego formatu.
*/
static void	ft_echo_args(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		i++;
		if (cmd[i])
			printf(" ");
	}
}
