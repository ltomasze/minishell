/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:06:07 by mbany             #+#    #+#             */
/*   Updated: 2024/12/14 16:58:04 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// wywołanie programu: cc main.c -lreadline -lhistory -lncurses

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Potrzebne dla strcmp i innych operacji na ciągach znaków
#include <unistd.h> // Potrzebne dla funkcji chdir
#include <sys/types.h> // Potrzebne dla fork i waitpid
#include <sys/wait.h> // Potrzebne dla waitpid
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

// Funkcja do obsługi polecenia "pwd"
void handle_pwd_command(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("Blad uzyskania sciezki biezacego katalogu");
    }
}

// Funkcja do obsługi polecenia "cd .."
void handle_cd_back_command(void)
{
    if (chdir("..") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Przeniesiono do katalogu nadrzednego: %s\n", cwd);
        }
        else
        {
            perror("Blad uzyskania sciezki po zmianie katalogu");
        }
    }
    else
    {
        perror("Blad podczas przechodzenia do katalogu nadrzednego");
    }
}

void handle_exit_command(char **args)
{
    int exit_status = 0;

    // Sprawdź, czy użytkownik podał argument dla exit
    if (args[1] != NULL)
    {
        // Zamień argument na liczbę całkowitą
        exit_status = atoi(args[1]);
        if (exit_status == 0 && strcmp(args[1], "0") != 0)
        {
            fprintf(stderr, "exit: nieprawidłowy argument: %s\n", args[1]);
            return;
        }
    }
    // Zakończ działanie programu
    printf("exit\n");
    rl_clear_history(); // Wyczyść historię readline
    free(args);         // Zwolnij pamięć na argumenty
    exit(exit_status);  // Zakończ program z podanym kodem
}

// Funkcja parsująca wprowadzone polecenie na tokeny
char **parse_command(char *input)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Błąd alokacji pamięci\n");
        exit(EXIT_FAILURE);
    }

    // Rozdzielenie ciągu na tokeny według spacji
    token = strtok(input, " ");
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        // Jeśli potrzeba więcej miejsca w buforze
        if (position >= bufsize)
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "Błąd alokacji pamięci\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " ");
    }
    tokens[position] = NULL;
    return tokens;
}

// Funkcja do wykonywania komend zewnętrznych
void execute_command(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Proces potomny: wykonaj polecenie
        if (execvp(args[0], args) == -1)
        {
            perror("Blad wykonania komendy");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Blad podczas tworzenia procesu
        perror("Blad forka");
    }
    else
    {
        // Proces macierzysty: czekaj na proces potomny
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
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

        // Parsowanie komendy na argumenty
        args = parse_command(command);
        // Sprawdź, czy polecenie to "pwd"
        if (args[0] && strcmp(args[0], "pwd") == 0)
        {
            handle_pwd_command();
        }
        // Sprawdź, czy polecenie to "cd .."
        else if (args[0] && strcmp(args[0], "cd") == 0 && args[1] && strcmp(args[1], "..") == 0)
        {
            handle_cd_back_command();
        }
        // Sprawdź, czy polecenie to "exit"
        if (args[0] && strcmp(args[0], "exit") == 0)
        {
            handle_exit_command(args);
        }
        else if (args[0])
        {
            // Wykonaj inne komendy jako komendy zewnętrzne
            execute_command(args);
        }

        // Zwróć pamięć po komendzie
        free(command);
        free(args);
    }
    // Wyczyść historię przed zakończeniem programu
    rl_clear_history();
    return (0);
}
