/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:34:11 by mbany             #+#    #+#             */
/*   Updated: 2025/01/25 17:27:09 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_cd_home(t_data *data, t_envp *home);
static int	cd_handler(char *str, t_data *data);
static int	ft_change_value(char *var, char *res, t_data *data);
static int	ft_cd_env_change(t_data *data, char *var, char *res);

/*
Funkcja `cd_bltin` obsługuje polecenie `cd` w shellu, 
zmieniając bieżący katalog roboczy. 
Jeśli podano więcej niż jeden argument, 
zwraca błąd „Too many arguments”. 
Jeśli brak argumentów, próbuje przejść do katalogu `HOME`, 
wyszukując go w zmiennych środowiskowych. Jeśli podano jeden argument, 
używa go do zmiany katalogu za pomocą funkcji `cd_handler`. 
W przypadku błędu zwraca kod `1`, a przy sukcesie `0`.
*/
int	cd_bltin(char **cmd, t_data *data)
{
	t_envp	*node;

	if (cmd[1] && cmd[2])
		return (ft_error_message("Too many arguments", 1));
	else if (!cmd[1])
	{
		node = fetch_envp_node(data->envp, "HOME");
		return (ft_cd_home(data, node));
	}
	else if (cd_handler(cmd[1], data) == -1)
		return (1);
	return (0);
}

/*
Funkcja `ft_cd_home` zmienia aktualny katalog roboczy 
na katalog domowy użytkownika zapisany w zmiennej środowiskowej `HOME`. 
Jeśli `HOME` nie jest ustawiona, funkcja zwraca błąd.
Po zmianie katalogu aktualizuje zmienne środowiskowe `OLDPWD` i `PWD`, 
które śledzą poprzedni oraz obecny katalog roboczy. 
Jest kluczowa w projekcie *Minishell*, 
ponieważ implementuje obsługę komendy `cd` bez argumentów, 
umożliwiając użytkownikowi szybki powrót do katalogu domowego 
zgodnie z zachowaniem standardowych powłok systemowych.
*/
static int	ft_cd_home(t_data *data, t_envp *home)
{
	t_envp	*node_pwd;
	char	curr[4096];

	if (!home)
		return (ft_error_message("HOME not set", 1));
	if (chdir(&home->value[5]))
	{
		ft_perror_message();
		return (1);
	}
	getcwd(curr, 4096);
	node_pwd = fetch_envp_node(data->envp, "PWD");
	if (!home)
	{
		if (ft_change_value("OLDPWD", NULL, data) == -1)
			return (1);
	}
	else
	{
		if (ft_change_value("OLDPWD", &node_pwd->value[4], data) == -1)
			return (1);
	}
	if (ft_change_value("PWD", curr, data) == -1)
		return (1);
	return (0);
}

/*
Funkcja `cd_handler` zmienia katalog roboczy na wskazany przez argument `str`. 
Jeśli zmiana katalogu nie powiedzie się, wyświetlany jest komunikat o błędzie, 
a funkcja zwraca `-1`. 
W przypadku sukcesu aktualizowane są zmienne środowiskowe `OLDPWD` 
(ustawiana na poprzednią lokalizację) i `PWD` (ustawiana na nową lokalizację). 
Funkcja jest potrzebna w projekcie *Minishell*, 
aby obsługiwać komendę `cd` z podanym argumentem 
i zapewniać prawidłową aktualizację zmiennych środowiskowych zgodnie 
z zachowaniem standardowych powłok systemowych.
*/
static int	cd_handler(char *str, t_data *data)
{
	char	cur[4096];
	t_envp	*node;

	if (chdir(str) == -1)
	{
		ft_perror_message();
		return (-1);
	}
	node = fetch_envp_node(data->envp, "PWD");
	if (node)
	{
		if (ft_change_value("OLDPWD", &node->value[4], data) == -1)
			return (-1);
	}
	getcwd(cur, 4096);
	if (ft_change_value("PWD", cur, data) == -1)
		return (-1);
	return (0);
}

/*
Funkcja `ft_change_value` zmienia wartość zmiennej środowiskowej 
o nazwie `var` na nową wartość `res`. 
Jeśli zmienna już istnieje, jej wartość jest aktualizowana, 
w przeciwnym przypadku tworzony jest nowy węzeł 
w liście zmiennych środowiskowych. 
Funkcja używa dynamicznie alokowanej pamięci 
do tworzenia nowego łańcucha znaków, 
który łączy nazwę zmiennej z jej nową wartością. 
Jest to istotne w projekcie *Minishell* 
do prawidłowego zarządzania zmiennymi środowiskowymi, 
w tym np. podczas obsługi zmiany katalogu roboczego (`cd`) 
i aktualizacji zmiennych takich jak `PWD` czy `OLDPWD`.
*/
static int	ft_change_value(char *var, char *res, t_data *data)
{
	t_envp	*node;
	char	*str;
	int		var_len;

	ft_cd_env_change(data, var, res);
	var_len = ft_strlen(var);
	node = fetch_envp_node(data->envp, var);
	str = malloc(sizeof(char) * (var_len + ft_strlen(res) + 2));
	if (!str)
		return (ft_perror_message());
	ft_strlcpy(str, var, var_len + 1);
	str[var_len] = '=';
	ft_strlcpy(&str[var_len + 1], res, ft_strlen(res) + 1);
	if (!node)
	{
		if (append_envp_node(&data->envp, str) == -1)
		{
			free(str);
			return (-1);
		}
		return (0);
	}
	free(node->value);
	node->value = str;
	return (0);
}

/*
Funkcja `ft_cd_env_change` usuwa zmienną środowiskową o nazwie `var` 
z listy zmiennych środowiskowych, jeśli jej nowa wartość (`res`) jest pusta. 
Najpierw sprawdza, czy zmienna już istnieje, a następnie, 
w zależności od jej pozycji w liście, 
usuwa ją jako głowę listy lub z innej pozycji. 
Jest używana w projekcie *Minishell* 
do usuwania nieaktualnych lub niepotrzebnych zmiennych środowiskowych, 
zwłaszcza podczas aktualizacji katalogu roboczego, 
gdzie może być konieczne usunięcie wcześniejszych wartości 
zmiennych takich jak `OLDPWD`.
*/
static int	ft_cd_env_change(t_data *data, char *var, char *res)
{
	t_envp	*node;

	if (res)
		return (0);
	node = fetch_envp_node(data->envp, var);
	if (!node)
		return (0);
	if (data->envp == node)
		ft_remove_head_node(&data->envp);
	else
		remove_envp_node(fetch_node_before(&data->envp, var));
	return (0);
}
