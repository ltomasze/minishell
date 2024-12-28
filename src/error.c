/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 12:00:29 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/28 13:46:08 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_error_message(char *str, int num)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	return (num);
}

/*ft_putstr_fd(str, 2);

Wywołuje funkcję ft_putstr_fd, która wypisuje ciąg znaków str do określonego deskryptora pliku.
Deskryptor 2 oznacza standardowe wyjście błędu (stderr).
Przykład: Jeśli str to "Error: invalid syntax", funkcja wypisze to na stderr.*/