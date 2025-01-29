/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:12:08 by mbany             #+#    #+#             */
/*   Updated: 2025/01/23 19:54:52 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_change_env(char *var, int j, t_data *data);

/*
Funkcja `export_bltin` obsługuje polecenie `export` wbudowane w shellu. 
Sprawdza, czy zostały podane zmienne do eksportu i czy mają poprawny format 
(rozpoczynają się literą lub podkreślnikiem, 
a pozostałe znaki przed `=` są alfanumeryczne lub podkreśleniem). 
Jeśli zmienna jest niepoprawna, wyświetla komunikat o błędzie i zwraca `1`. 
Dla poprawnych zmiennych wywołuje `ft_change_env`, aby dodać je do środowiska. 
Funkcja kończy się sukcesem (`0`) lub błędem (`1`) 
w przypadku niepoprawnych zmiennych lub innych problemów.
*/
int	export_bltin(char **cmd, t_data *data)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (data->cmd->next || !cmd[1])
		return (1);
	while (cmd[i])
	{
		if (!ft_isalpha(cmd[i][j]) && cmd[i][j] != '_')
			return (ft_error_message("Export: not a valid identifier", 1));
		j++;
		while (cmd[i][j] && cmd[i][j] != '=')
		{
			if (!ft_isalnum(cmd[i][j]) && cmd[i][j] != '_')
				return (1);
			j++;
		}
		if (ft_change_env(cmd[i], j, data) == -1)
			return (1);
		i++;
		j = 0;
	}
	return (0);
}

/*
Funkcja `ft_change_env` zajmuje się modyfikowaniem 
lub dodawaniem zmiennych środowiskowych. Sprawdza, 
czy przekazana zmienna zawiera znak `=`, 
oddzielający nazwę zmiennej od jej wartości. 
Następnie, przy użyciu `ft_substr`, wyodrębnia nazwę zmiennej. 
Jeżeli zmienna już istnieje w `envp`, 
jej wartość jest zastępowana nową (przekazaną w `var`). 
Jeśli zmienna nie istnieje, jest dodawana do listy `envp`. 
W przypadku błędu przy alokacji pamięci lub operacjach na liście, 
funkcja zwraca `-1`. 
Po przetworzeniu zmiennej odpowiednio zwalnia pamięć 
dla nazw zmiennych i przypisuje nowe wartości.
*/
static int	ft_change_env(char *var, int j, t_data *data)
{
	t_envp	*node;
	char	*var_name;
	char	*dup_var;

	if (var[j] != '=')
		return (0);
	var_name = ft_substr(var, 0, j);
	if (!var_name)
		return (ft_perror_message());
	dup_var = ft_strdup(var);
	if (!dup_var)
		return (ft_perror_message());
	node = fetch_envp_node(data->envp, var_name);
	free(var_name);
	if (!node)
	{
		if (append_envp_node(&data->envp, dup_var) == -1)
			return (-1);
		return (0);
	}
	free(node->value);
	node->value = dup_var;
	return (0);
}

/*
Funkcja `ft_print_env_var` realizuje działanie wbudowanej komendy `export`, 
wypisując wszystkie zmienne środowiskowe przechowywane w strukturze `envp` 
w formacie `declare -x VAR=value`. Jeśli przekazano argument do komendy, 
funkcja kończy działanie z kodem `cmd_exit_status`. 
W przeciwnym razie przechodzi po wszystkich zmiennych środowiskowych 
i wypisuje je. Funkcja kończy działanie z kodem `0`. 
Jest to część projektu *minishell*, 
odwzorowująca funkcjonalność komendy `export` w tym interpreteurze.
*/
int	ft_print_env_var(t_data *data)
{
	t_envp	*envp;

	if (data->cmd->cmd[1])
		exit(data->cmd_exit_status);
	envp = data->envp;
	while (envp)
	{
		printf("declare -x %s\n", envp->value);
		envp = envp->next;
	}
	exit (0);
}
