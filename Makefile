# Nazwa programu
NAME = minishell

# Kompilator i flagi
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Ścieżki do folderów
SRCS_DIR = src
OBJS_DIR = obj
INCLUDES = -Iincludes

# Pliki źródłowe i obiektowe
SRCS = $(SRCS_DIR)/main.c $(SRCS_DIR)/parser.c $(SRCS_DIR)/executor.c
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

# Libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Flagi dla bibliotek
LIBS = -lreadline -lncurses

# Reguła domyślna
all: $(NAME)

# Kompilacja programu
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS) $(LIBFT)

# Kompilacja plików obiektowych
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Tworzenie folderu na pliki obiektowe
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Kompilacja Libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Usuwanie plików obiektowych
clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)
	rm -rf $(OBJS_DIR)

# Usuwanie plików obiektowych i programu
fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

# Rekompilacja programu
re: fclean all

# Ignorowanie plików dla make
.PHONY: all clean fclean re
