/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:36:52 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/22 13:37:56 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_check_access(char *file, int type)
{
	int	value;

	if (type == READ)
	{
		value = access(file, F_OK | R_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	else if (type == WRITE)
	{
		value = access(file, F_OK | W_OK);
		if (value < 0)
			return (ft_perror_message());
	}
	return (0);
}