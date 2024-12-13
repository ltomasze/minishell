#include <stdio.h> //perror
#include <stdlib.h> //getenv, exit
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>  // chdir()
#include <string.h> // strcmp

int my_cd(char **args)
{
    // Sprawdzenie, czy zmienna środowiskowa HOME jest ustawiona
    if (args[1] == NULL)
    {
        // Jeśli nie podano argumentu, przejdź do katalogu domowego
        char *home = getenv("HOME");
        if (home == NULL) 
        {
            // Jeśli HOME jest NULL, wypisujemy komunikat o błędzie
            write(2, "Błąd: zmienna środowiskowa HOME nie jest ustawiona.\n", 52);
            return 1;
        }
        if (chdir(home) != 0)
        {
            write(2, "Błąd: Nie udało się przejść do katalogu domowego.\n", 53);
            return 1;
        }
    } 
    else
    {
        // Inaczej zmień katalog na podany w args[1]
        if (access(args[1], F_OK) != 0)
        {  // Sprawdzamy, czy katalog istnieje
            write(2, "Błąd: Podany katalog nie istnieje.\n", 35);
            return 1;
        }
        if (chdir(args[1]) != 0)
        {
            write(2, "Błąd: Nie udało się przejść do katalogu.\n", 41);
            return 1;
        }
    }
    return 0;
}

int my_exit(char **args)
{
    // Zwrócenie 0 jako kod zakończenia (można to zmienić w zależności od sytuacji)
    return 1;  // Wartość 1 oznacza zakończenie pętli w main() i zamknięcie powłoki
}

// Funkcja do obliczania liczby argumentów w komendzie
int count_args(char *command)
{
    int count = 0;
    int i = 0;

    while (command[i] != '\0')
    {
        // Pomiń spacje i tabulatory
        while (command[i] != '\0' && (command[i] == ' ' || command[i] == '\t'))
            i++;
        // Jeśli napotkaliśmy koniec ciągu, zakończ
        if (command[i] == '\0')
            break;
        // Zainicjuj nowy argument
        count++;
        // Szukaj końca bieżącego argumentu
        while (command[i] != '\0' && command[i] != ' ' && command[i] != '\t')
            i++;
    }
    return count;
}

void parse_command(char *command, char **args)
{
    int i = 0, j = 0;
    int count = count_args(command);  // Liczymy liczbę argumentów
    *args = malloc((count + 1) * sizeof(char *)); // Alokujemy tablicę na argumenty

    while (command[i] != '\0')
    {
        // Pomiń spacje i tabulatory
        while (command[i] != '\0' && (command[i] == ' ' || command[i] == '\t'))
            i++;
        // Jeśli napotkaliśmy koniec ciągu, zakończ
        if (command[i] == '\0')
            break;
        // Zainicjuj nowy argument
        char *current_arg = &command[i];
        // Szukaj końca bieżącego argumentu (po napotkaniu spacji lub końca ciągu)
        while (command[i] != '\0' && command[i] != ' ' && command[i] != '\t')
            i++;
        // Zakończ argument NULL
        command[i] = '\0';
        (*args)[j] = current_arg;
        j = j + 1;  // Dodaj argument do tablicy
    }
    (*args)[j] = NULL;  // Zakończ tablicę NULL
}

// Funkcja do wczytywania komendy z readline
char *get_command(void) 
{
    // Wyświetl prompt i wczytaj linię
    char *command = readline("$ ");
    // Jeśli wprowadzono komendę, dodaj ją do historii
    if (command != NULL && command[0] != '\0')
	{
        add_history(command);
    }
    return (command); // readline alokuje pamięć, którą musimy później zwolnić
}

int main(void)
{
    char *command;
    char **args;

    while (1)
	{
        // Pobierz komendę od użytkownika
        command = get_command();
        // Jeśli komenda jest NULL (np. EOF), zakończ program
        if (command == NULL)
		{
            printf("exit\n");
            break;
        }
        // Parsuj komendę na argumenty
        parse_command(command, &args);
         // Obsłuż komendę
        if (args[0] != NULL)
        {
            if (strcmp(args[0], "cd") == 0) 
            {
                if (my_cd(args) != 0) 
                    // Obsługa błędów, np. brak katalogu
                    perror("cd error");
            }
            else if (strcmp(args[0], "exit") == 0) 
                break;  // Zakończ program
            else
            {
                handle_other_commands(args);  // Obsługuje inne komendy
            }
        }
        // Zwróć pamięć po komendzie i argumentach
        free(command);
        free(args);  // Zwróć pamięć przydzieloną dla tablicy argumentów
    }
    // Wyczyść historię przed zakończeniem programu
    rl_clear_history();
    return 0;
}

/*Uwagi:
-usuń funkcje handle_other_commands,
-zamień funkcje strcmp na ft_strcmp
*/


// wywołanie programu: cc main.c -lreadline -lhistory -lncurses