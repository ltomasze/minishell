/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:33:56 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:46:32 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_ft_split(char **split)
{
	char	**orig_split;

	orig_split = split;
	while (*split != NULL)
	{
		free(*split);
		split++;
	}
	free(orig_split);
}

void	ft_free_commands(t_cmd **commands)
{
	t_cmd	*temp;

	if (!commands)
		return ;
	while (*commands)
	{
		temp = *commands;
		if (temp->infile)
			free(temp->infile);
		if (temp->outfile)
			free(temp->outfile);
		if (temp->cmd)
			free_ft_split(temp->cmd);
		*commands = (*commands)->next;
		free(temp);
	}
}