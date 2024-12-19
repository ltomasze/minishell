/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:42:37 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/19 17:30:45 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(char *command)
{
    if (ft_strcmp(command, "pwd") == 0)
    {
        pwd_command();
    }
    else if (ft_strcmp(command, "exit") == 0)
        return;
    else
    {
        print_error("msh", "command not found");
    }
}
