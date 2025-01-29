/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:24:26 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 16:59:04 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	msg_error(char *err)
{
	write(2, err, ft_strlen(err));
	write(2, "\n", 1);
}

int	ft_error_message(char *str, int num)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	return (num);
}

int	ft_perror_message(void)
{
	perror(NULL);
	return (-1);
}

/*
Funkcja `ft_perror_free` wypisuje komunikat 
o błędzie za pomocą `ft_perror_message`, 
a następnie zwalnia pamięć wskazywaną 
przez wskaźniki `first`, `second` i `third`, 
jeśli nie są one puste. Na końcu zwraca `-1`, aby zasygnalizować błąd. 
Funkcja służy do obsługi błędów i jednoczesnego czyszczenia 
zaalokowanej pamięci, zapobiegając wyciekom.
*/
int	ft_perror_free(char *first, char *second, char *third)
{
	ft_perror_message();
	if (first)
		free(first);
	if (second)
		free(second);
	if (third)
		free(third);
	return (-1);
}
