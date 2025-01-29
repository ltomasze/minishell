/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands01.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:04:18 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 18:48:39 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_create_cmds(t_token *tokens, t_cmd *commands, int i);
static int	ft_free_args(int i, char **cmds);

/*
Funkcja `ft_count_tok` liczy liczbę tokenów typu 
T_WORD` (słowo) w łańcuchu tokenów, zatrzymując się, 
gdy napotka token typu `T_PIPE`. 
Działa w następujący sposób: 
zaczyna od pierwszego tokena, sprawdza jego typ, 
a jeśli jest różny od `T_WORD`, 
przeskakuje o dwa tokeny do przodu 
(przeskakuje przez inne typy tokenów), 
jeśli napotka token `T_WORD`, inkrementuje licznik, 
zmienia jego typ na `T_ARG`, 
a następnie przechodzi do następnego tokena. 
Funkcja zwraca liczbę tokenów `T_WORD` przed napotkaniem `T_PIPE`.
*/
static int	ft_count_tok(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type != T_WORD)
		{
			tokens = tokens->next->next;
			continue ;
		}
		i++;
		tokens->type = T_ARG;
		tokens = tokens->next;
	}
	return (i);
}

/*
Funkcja `ft_command` analizuje token wskazywany 
przez `*cur_token` i przetwarza go jako część polecenia. 
Jeśli token ma typ `T_WORD`, liczy kolejne związane tokeny 
(`ft_count_tok`), a następnie tworzy polecenie w bieżącej 
strukturze `t_cmd` za pomocą `ft_create_cmds`. 
W razie błędu zwalnia pamięć dla tokenów i poleceń oraz zwraca błąd. 
Jeśli token ma typ `T_ARG`, przeskakuje przez wszystkie argumenty, 
aktualizując wskaźnik `*cur_token`. 
Funkcja zwraca `0`, gdy zakończy swoje działanie bez błędów.
*/
int	ft_command(t_token **cur_token, t_token *tokens,
t_cmd **cur_command, t_cmd *cmds)
{
	int	i;

	if (!(*cur_token))
		return (0);
	if ((*cur_token)->type == T_WORD)
	{
		i = ft_count_tok(*cur_token);
		if (ft_create_cmds(*cur_token, *cur_command, i) == 1)
		{
			ft_free_tokens(&tokens);
			ft_free_commands(&cmds);
			return (-1);
		}
	}
	if ((*cur_token)->type == T_ARG)
	{
		while (*cur_token && (*cur_token)->type == T_ARG)
			*cur_token = (*cur_token)->next;
	}
	return (0);
}
/*
Funkcja `ft_create_cmds` tworzy tablicę argumentów 
dla polecenia, przetwarzając tokeny typu 
`T_ARG` aż do napotkania tokenu typu `T_PIPE`. 
Dla każdego tokena typu `T_ARG` tworzy jego kopię 
i zapisuje ją w tablicy `cmds`. Jeśli napotka błąd 
(np. nie uda się skopiować tekstu tokenu), zwalnia 
już przydzieloną pamięć i kończy działanie funkcji. 
Na końcu przypisuje tablicę argumentów do struktury 
polecenia (`commands->cmd`) i zwraca `0`, oznaczając sukces.
*/

static int	ft_create_cmds(t_token *tokens, t_cmd *commands, int i)
{
	char	**cmds;
	char	*arg;
	int		n;

	n = 0;
	cmds = malloc(sizeof(char *) * (i + 1));
	if (!cmds)
		return (ft_perror_message());
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type != T_ARG)
		{
			tokens = tokens->next;
			continue ;
		}
		arg = ft_strdup(tokens->text);
		if (!arg)
			return (ft_free_args(n, cmds));
		cmds[n++] = arg;
		tokens = tokens->next;
	}
	cmds[n] = NULL;
	commands->cmd = cmds;
	return (0);
}
/*
Funkcja `ft_free_args` zwalnia pamięć przydzieloną 
dla tablicy argumentów `cmds`. Iteruje po elementach 
tablicy, zwalniając każdą z jej pozycji, a potem 
zwalnia samą tablicę. Na początku wywołuje funkcję 
`ft_perror_message`, prawdopodobnie w celu 
wyświetlenia komunikatu o błędzie, 
a na końcu zwraca `-1`, sygnalizując błąd.
*/

static int	ft_free_args(int i, char **cmds)
{
	int	j;

	ft_perror_message();
	j = 0;
	while (j < i)
		free(cmds[j++]);
	free(cmds);
	return (-1);
}
