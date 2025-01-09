/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 12:00:29 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/09 17:30:01 by ltomasze         ###   ########.fr       */
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
/* perror wypisuje standardowe komunikaty w zalezności od wartości errno
np na mallocku otrzymujac wartość null wypisze "Cannot allocate memory"
p.s. null w nawiasie to nie wynik tylko że poza standardowym odpowiedzią ma
nic więcej nie dopisać, gdyby np. było perror(Error) to bedzie komunikat
Error: Cannot allocate memory */
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

/*ft_putstr_fd(str, 2);

Wywołuje funkcję ft_putstr_fd, która wypisuje ciąg znaków str do określonego deskryptora pliku.
Deskryptor 2 oznacza standardowe wyjście błędu (stderr).
Przykład: Jeśli str to "Error: invalid syntax", funkcja wypisze to na stderr.*/