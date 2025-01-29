/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:38:07 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/26 10:50:29 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Funkcja `remove_envp_node` usuwa węzeł listy zmiennych środowiskowych `envp`, 
który znajduje się za podanym węzłem `prev_node`. 
Aktualizuje wskaźnik `next` w `prev_node`, 
aby pominąć usuwany węzeł, 
a następnie zwalnia pamięć zajmowaną przez jego wartość i sam węzeł. 
Funkcja zapobiega wyciekowi pamięci i zachowuje ciągłość listy.
*/
void	remove_envp_node(t_envp *prev_node)
{
	t_envp	*node;

	node = prev_node->next;
	if (node->next != NULL)
		prev_node->next = node->next;
	else
		prev_node->next = NULL;
	free (node->value);
	free (node);
}

/*
Funkcja `fetch_node_before` wyszukuje węzeł poprzedzający węzeł w liście `envp`, 
który zawiera zmienną środowiskową o nazwie zgodnej z podanym kluczem `key`. 
Oblicza długość klucza, a następnie iteruje po liście, 
porównując `key` z nazwami zmiennych w węzłach. 
Jeśli zmienna znajduje się w pierwszym węźle, zwraca jego wskaźnik. 
Jeśli zmienna jest dalej w liście, 
zwraca wskaźnik na węzeł bezpośrednio poprzedzający. 
Jeśli zmiennej nie ma, zwraca `NULL`. 
Funkcja służy do modyfikowania listy przez umożliwienie 
usuwania lub aktualizacji znalezionego węzła.
*/
t_envp	*fetch_node_before(t_envp **head, char *key)
{
	t_envp	*p_node;
	int		key_len;

	key_len = ft_strlen(key);
	if (!(ft_strncmp(key, (*head)->value, key_len))
		&& (*head)->value[key_len] == '=')
		return (*head);
	p_node = *head;
	while (p_node)
	{
		if (!p_node->next)
			return (NULL);
		else if (!p_node->next->value)
			p_node = p_node->next;
		else if (!(ft_strncmp(key, p_node->next->value, key_len))
			&& p_node->next->value[key_len] == '=')
			return (p_node);
		else
			p_node = p_node->next;
	}
	return (NULL);
}

/*
Funkcja `convert_envp_llist_to_array` konwertuje 
listę połączoną zmiennych środowiskowych na tablicę łańcuchów znaków. 
Iteruje po liście, kopiując wartości każdej zmiennej 
do nowej tablicy i dodaje na końcu wskaźnik `NULL`. 
Funkcja jest używana w projekcie *Minishell*, 
aby dostosować format zmiennych środowiskowych do wymagań funkcji systemowych, 
które oczekują tablicy jako argumentu, 
np. podczas wykonywania zewnętrznych poleceń.
*/
char	**convert_envp_list_to_array(t_envp *head)
{
	t_envp	*node;
	size_t	nodes_cnt;
	size_t	i;
	char	**arr;

	nodes_cnt = 0;
	node = head;
	while (node && ++nodes_cnt)
		node = node->next;
	arr = malloc(sizeof(char *) * (nodes_cnt + 1));
	if (!arr)
		return (NULL);
	node = head;
	i = 0;
	while (node)
	{
		arr[i] = ft_strdup(node->value);
		if (!arr[i])
			return (NULL);
		i++;
		node = node->next;
	}
	arr[i] = NULL;
	return (arr);
}
