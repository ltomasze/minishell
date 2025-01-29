/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize01.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 13:34:28 by mbany             #+#    #+#             */
/*   Updated: 2025/01/26 11:05:41 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `optimize_str_final` tworzy nową 
kopię łańcucha znaków `str_final` za pomocą `ft_strdup`, 
zwalnia pamięć zajmowaną przez oryginalny łańcuch 
i zwraca wskaźnik do nowej kopii. 
Służy do zarządzania pamięcią, 
aby zastąpić oryginalny wskaźnik nowym, 
niezależnym łańcuchem znaków.
*/
static char	*optimize_str_final(char *str_final)
{
	char	*new_str;

	if (!str_final)
		return (NULL);
	new_str = ft_strdup(str_final);
	free(str_final);
	if (!new_str)
		return (NULL);
	return (new_str);
}

/*
Funkcja `sanitize_line` czyści wprowadzoną 
linię tekstu w strukturze `data` poprzez 
usunięcie początkowych i końcowych spacji 
oraz tabulatorów za pomocą `ft_strtrim`, 
tworzy dynamicznie pamięć na zaktualizowany łańcuch o podwójnej długości, 
przetwarza ten łańcuch za pomocą `process_str`, 
a następnie optymalizuje go przy użyciu `optimize_str_final`. 
W razie błędów (np. alokacji pamięci) wyświetla komunikat 
o błędzie i zwalnia zajętą pamięć. 
Funkcja służy do przygotowania wprowadzonego tekstu do dalszego przetwarzania.
*/
void	sanitize_line(t_data *data)
{
	char	*str;
	int		str_len;

	str = ft_strtrim(data->line, " \t");
	free(data->line);
	if (!str)
		return ;
	str_len = ft_strlen(str) * 2;
	data->line = malloc(sizeof(char) * (ft_strlen(str) * 2));
	if (!data->line)
	{
		free(str);
		msg_error(SANITIZE_ERROR);
		return ;
	}
	data->line = process_str(str, data->line, str_len);
	data->line = optimize_str_final(data->line);
	if (!data->line)
	{
		free(str);
		msg_error(SANITIZE_ERROR);
		return ;
	}
	free(str);
}
