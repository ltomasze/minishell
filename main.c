#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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
        // Wydrukuj wprowadzoną komendę (do debugowania)
        printf("Wprowadzone polecenie: %s\n", command);
        // Zwróć pamięć po komendzie
        free(command);
    }
    // Wyczyść historię przed zakończeniem programu
    rl_clear_history();
    return (0);
}
// wywołanie programu: cc main.c -lreadline -lhistory -lncurses