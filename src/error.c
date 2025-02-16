/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:37:58 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:38:30 by ltomasze         ###   ########.fr       */
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
The function `ft_perror_free` prints an error message using `ft_perror_message`,  
and then frees the memory pointed to by the `first`, `second`, and `third` pointers, if they are not NULL.  
Finally, it returns `-1` to signal an error.  
This function is used for error handling and simultaneously clearing allocated memory, preventing memory leaks.
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
