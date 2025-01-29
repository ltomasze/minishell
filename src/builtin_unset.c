/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:23:19 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 16:22:19 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `unset_bltin` usuwa zmienne środowiskowe wskazane 
w tablicy `cmd` z listy `envp`. 
Dla każdego argumentu tworzy jego nazwę (część przed znakiem `=`) 
za pomocą `ft_substr`. Następnie szuka zmiennej w liście `envp` 
za pomocą `fetch_envp_node`. Jeśli zmienna istnieje 
i znajduje się na początku listy, usuwa ją przez `ft_remove_head_node`. 
W przeciwnym razie usuwa odpowiedni węzeł, 
znajdując jego poprzednika za pomocą `fetch_node_before`. 
Po przetworzeniu każdego argumentu zwalnia pamięć dla nazwy 
i kończy sukcesem (`0`). Jeśli napotka błąd, wywołuje `ft_perror_message`.
*/
int	unset_bltin(char **cmd, t_data *data)
{
	int		i;
	int		j;
	char	*name;
	t_envp	*node;

	i = 1;
	j = 0;
	while (cmd[i])
	{
		while (cmd[i][j] && cmd[i][j] != '=')
			j++;
		name = ft_substr(cmd[i], 0, j);
		if (!name)
			return (ft_perror_message());
		node = fetch_envp_node(data->envp, name);
		if (!ft_strncmp(name, data->envp->value, j)
			&& data->envp->value[j] == '=')
			ft_remove_head_node(&(data->envp));
		else if (node)
			remove_envp_node(fetch_node_before(&data->envp, name));
		i++;
		j = 0;
		free(name);
	}
	return (0);
}

/*
Funkcja `ft_remove_head_node` usuwa pierwszy węzeł 
listy zmiennych środowiskowych `envp`. 
Przechowuje wskaźnik na następny węzeł, 
zwalnia pamięć zajmowaną przez wartość i sam pierwszy węzeł, 
a następnie ustawia wskaźnik głowy listy na kolejny węzeł. 
Dzięki temu lista jest modyfikowana bez utraty danych poza usuniętym elementem.
*/
void	ft_remove_head_node(t_envp **head)
{
	t_envp	*new_head;

	new_head = (*head)->next;
	free((*head)->value);
	free(*head);
	*head = new_head;
}
