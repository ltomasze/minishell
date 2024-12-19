/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:02:40 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/19 17:03:08 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))  // Dopóki znaki są równe i nie dotarliśmy do końca ciągu
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;  // Zwracamy różnicę pierwszych różnych znaków
}
