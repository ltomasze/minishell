42 Warsaw Minishell project <br><br>


<b>DONE</b><br>

Krok 1: Podstawowe struktury i inicjalizacja projektu
Zaimplementuj strukturę t_data:
Zawiera wskaźnik na line (aktualnie wczytana linia z terminala).
Zawiera wskaźnik na zmienne środowiskowe (envp) oraz inne potrzebne dane.
Napisz funkcję init:
Inicjalizuje strukturę t_data.
Kopiuje zmienne środowiskowe do własnego formatu (np. lista lub tablica).
Funkcja free_resources:
Zwalnia pamięć zajętą przez dane w strukturze t_data.
Po tym etapie program się kompiluje, wczytuje linie (ale nic nie robi z nimi) i poprawnie kończy działanie.



<b>UNDER CONSTRUCTION</b>

Krok 2: Obsługa sygnałów
Zaimplementuj funkcję handle_signals:
Obsługuje Ctrl+C (SIGINT), aby nie zakończyć programu, lecz wyczyścić linię wejściową i wyświetlić nowy prompt.
Ignoruje Ctrl+\\ (SIGQUIT).
Zintegrowana z pętlą główną w miejscu, w którym jest używana.
Program nadal powinien się kompilować i teraz prawidłowo obsługiwać Ctrl+C oraz Ctrl+\.

Krok 3: Wczytywanie linii wejściowej
Napisz funkcję read_line, która:
Wyświetla prompt i wczytuje linię z terminala (np. z użyciem readline).
Dodaje linię do historii, jeśli nie jest pusta.
Zwraca linię wejściową w strukturze t_data.
Obsługa pustych linii:
Jeśli linia jest pusta, pętla powinna kontynuować działanie bez dodatkowych akcji.
Program po tym etapie wczytuje linie od użytkownika, dodaje je do historii i ignoruje puste wiersze.

Krok 4: Analiza składni i walidacja (parser)
Funkcja check_syntax:
Sprawdza poprawność linii wejściowej:
Zamknięte cudzysłowy (np. " i ').
Poprawne metaznaki (np. brak podwójnych ||, && bez kontekstu, itp.).
Jeśli składnia jest niepoprawna, program powinien wyświetlać stosowny komunikat i kontynuować pracę.
Jeśli składnia jest poprawna, linia jest przekazywana dalej.
Krok 5: Czyszczenie i normalizacja linii
Zaimplementuj funkcję sanitaze_line, która:
Usuwa nadmiarowe spacje i tabulatory.
Zastępuje zmienne środowiskowe ($VAR) ich wartościami.
Normalizuje linię w sposób ułatwiający jej późniejsze przetwarzanie.
Program na tym etapie wczytuje i czyści linie do dalszej obróbki.

Krok 6: Tworzenie komend
Napisz funkcję ft_cmds_creation, która:
Dzieli linię wejściową na polecenia i argumenty (np. z użyciem ;, |, lub znaków redirekcji).
Tworzy strukturę reprezentującą komendy (np. lista poleceń z argumentami).
Obsługuje różne typy metaznaków:
Redirekcje (<, >, <<, >>).
Potoki (|).
Krok 7: Wykonywanie wbudowanych poleceń
Dodaj funkcję check_for_builtins, która obsługuje polecenia:
echo z flagą -n.
cd (relatywne i absolutne ścieżki).
pwd (bez argumentów).
export, unset, env (w podstawowej formie).
exit (bez argumentów).
Na tym etapie podstawowe wbudowane polecenia działają.

Krok 8: Wykonywanie poleceń zewnętrznych
Funkcja execute_cmds, która:
Wykonuje komendy zewnętrzne (np. ls, cat).
Obsługuje potoki (|) poprzez tworzenie procesów potomnych.
Obsługuje redirekcje wejścia/wyjścia (<, >).
Krok 9: Zaawansowana obsługa sygnałów
Rozbuduj obsługę sygnałów:
Dostosuj SIGINT i SIGQUIT w procesach potomnych.
Ignoruj SIGQUIT w procesie głównym.
