# Nazwa programu
NAME = minishell

# Kompilator i flagi
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Pliki źródłowe i obiektowe
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Flagi dla bibliotek
LIBS = -lreadline -lhistory -lncurses

# Reguła domyślna
all: $(NAME)

# Kompilacja programu
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

# Reguła do kompilacji plików obiektowych
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Usuwanie plików obiektowych
clean:
	rm -f $(OBJS)

# Usuwanie plików obiektowych i programu
fclean: clean
	rm -f $(NAME)

# Rekompilacja programu
re: fclean all

# Ignoruj plik makefile i inne pliki dla polecenia `make clean`
.PHONY: all clean fclean re