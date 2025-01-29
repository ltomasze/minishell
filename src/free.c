/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:33:56 by ltomasze          #+#    #+#             */
/*   Updated: 2025/01/25 16:54:02 by mbany            ###   ########.fr       */
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

/*
Funkcja ft_free_commands jest odpowiedzialna 
za zwolnienie pamięci związanej z listą poleceń (t_cmd), w tym:
Zwalnia wszystkie tablice argumentów cmd.
Zwalnia pamięć przypisaną do plików wejściowych (infile) i wyjściowych (outfile).
Iteruje po całej liście poleceń i zwalnia pamięć przypisaną do każdego elementu.
Jest to istotna funkcja w zarządzaniu pamięcią w programie, 
ponieważ pozwala uniknąć wycieków pamięci poprzez zapewnienie, 
że wszystkie elementy struktury są odpowiednio zwolnione 
po zakończeniu ich użycia.
Pokażmy przykład, jak funkcja ft_free_commands działa 
na rzeczywistej liście poleceń (t_cmd), zakładając, 
że mamy kilka poleceń w minishellu. 
Załóżmy, że w programie mamy dwa polecenia: 
jedno z plikiem wejściowym i wyjściowym, 
a drugie z argumentami.

Przykład listy poleceń t_cmd
Załóżmy, że nasza lista t_cmd wygląda tak:

t_cmd *cmd1 = malloc(sizeof(t_cmd));
t_cmd *cmd2 = malloc(sizeof(t_cmd));
t_cmd *cmd3 = malloc(sizeof(t_cmd));

cmd1->cmd = malloc(sizeof(char*) * 3);
cmd1->cmd[0] = strdup("ls");
cmd1->cmd[1] = strdup("-l");
cmd1->cmd[2] = NULL;
cmd1->infile = strdup("input.txt");
cmd1->outfile = strdup("output.txt");
cmd1->next = cmd2;

cmd2->cmd = malloc(sizeof(char*) * 2);
cmd2->cmd[0] = strdup("cat");
cmd2->cmd[1] = NULL;
cmd2->infile = NULL;
cmd2->outfile = strdup("output2.txt");
cmd2->next = cmd3;

cmd3->cmd = malloc(sizeof(char*) * 2);
cmd3->cmd[0] = strdup("grep");
cmd3->cmd[1] = strdup("error");
cmd3->next = NULL;
cmd3->infile = NULL;
cmd3->outfile = NULL;
Stan przed zwolnieniem pamięci:

Pierwsze polecenie (cmd1):

Komenda: ls -l
Plik wejściowy: input.txt
Plik wyjściowy: output.txt
Drugie polecenie (cmd2):

Komenda: cat
Plik wyjściowy: output2.txt
Trzecie polecenie (cmd3):

Komenda: grep error
Przykład działania funkcji ft_free_commands
Załóżmy, że chcemy zwolnić całą pamięć, 
która została przydzielona dla tej listy poleceń,
 wywołując funkcję ft_free_commands(&cmd1).

Co się wydarzy w funkcji ft_free_commands?
Sprawdzanie, czy lista nie jest pusta:

Jeśli lista nie jest pusta, przechodzimy do kolejnych kroków.
Iterowanie przez listę poleceń: 
Pętla while (*commands) iteruje przez wszystkie elementy w liście,
 aż do momentu, gdy napotka NULL.

Pierwsza iteracja (cmd1):

temp = *commands przypisuje wskaźnik do cmd1.
Sprawdzamy, czy są przypisane pliki infile i outfile:
Zwalniamy pamięć dla infile (input.txt).
Zwalniamy pamięć dla outfile (output.txt).
Zwalniamy pamięć dla cmd (tablica: ["ls", "-l", NULL]) za pomocą free_ft_split.
Przechodzimy do następnego elementu listy: 
*commands = (*commands)->next (czyli cmd2).
Druga iteracja (cmd2):

temp = *commands przypisuje wskaźnik do cmd2.
Zwalniamy pamięć dla outfile (output2.txt).
Zwalniamy pamięć dla cmd (tablica: ["cat", NULL]) za pomocą free_ft_split.
Przechodzimy do następnego elementu listy: 
*commands = (*commands)->next (czyli cmd3).
Trzecia iteracja (cmd3):

temp = *commands przypisuje wskaźnik do cmd3.
Zwalniamy pamięć dla cmd (tablica: 
["grep", "error", NULL]) za pomocą free_ft_split.
Przechodzimy do końca listy: *commands = (*commands)->next (czyli NULL).
Zwalnianie pamięci dla samych struktur t_cmd: 
Po zwolnieniu wszystkich pól w strukturach (argumentów, 
plików wejściowych i wyjściowych), zwalniamy pamięć, 
która została przydzielona na same struktury t_cmd za pomocą free(temp).

Wynik po wywołaniu funkcji:
Po wywołaniu ft_free_commands(&cmd1):

Zostaną zwolnione następujące zasoby:

Pamięć dla pliku wejściowego i wyjściowego każdego polecenia.
Pamięć dla argumentów (tablicy cmd) każdego polecenia.
Pamięć przydzielona na same struktury t_cmd.
Lista poleceń zostanie całkowicie zwolniona, 
a wskaźnik cmd1 będzie wskazywał na NULL.

Wizualizacja procesu:
Przed zwolnieniem pamięci:

cmd1 -> [cmd = ["ls", "-l", NULL], infile = "input.txt", 
	outfile = "output.txt", next = cmd2]
cmd2 -> [cmd = ["cat", NULL], infile = NULL, 
	outfile = "output2.txt", next = cmd3]
cmd3 -> [cmd = ["grep", "error", NULL], 
	infile = NULL, outfile = NULL, next = NULL]
Po zwolnieniu pamięci:

cmd1 -> NULL
cmd2 -> NULL
cmd3 -> NULL
Cała pamięć została zwolniona, 
a wskaźniki do kolejnych poleceń w liście zostały ustawione na NULL, 
więc nie ma już żadnej aktywnej pamięci związanej z listą poleceń.


Funkcja free_ft_split służy do zwolnienia pamięci przydzielonej 
dla tablicy wskaźników (char **split), 
która zawiera łańcuchy znaków (np. wynik działania funkcji ft_split).
Zwalnia ona zarówno pojedyncze łańcuchy znaków 
(np. "ls", "-l", "/home/user") oraz
 samą tablicę wskaźników.Funkcja działa w następujący sposób:

orig_split = split: Kopiujemy wskaźnik split do zmiennej orig_split, 
aby później zwolnić pamięć dla całej tablicy wskaźników.
while (*split != NULL): Pętla iteruje przez wszystkie elementy tablicy. 
Dla każdego elementu wywołuje funkcję free, 
aby zwolnić pamięć przypisaną do poszczególnych łańcuchów znaków.
free(orig_split): Po zwolnieniu pamięci dla każdego łańcucha w tablicy 
zwalniamy pamięć zajmowaną przez samą tablicę (wskaźniki).
*/