/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:19:05 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/19 17:20:59 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_error(const char *prefix, const char *msg)
{
    // Wypisanie komunikatu na standardowe wyjście błędów (stderr) za pomocą write
    write(2, prefix, strlen(prefix));   // Prefix (np. "msh")
    write(2, ": ", 2);                  // Dodanie ": "
    write(2, msg, strlen(msg));         // Wiadomość o błędzie
    write(2, "\n", 1);                  // Dodanie nowej linii
}