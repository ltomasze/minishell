# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/17 14:51:20 by ltomasze          #+#    #+#              #
#    Updated: 2024/12/17 16:20:25 by ltomasze         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
SRCS = $(SRCS_DIR)/main.c 
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

# Flagi dla bibliotek
LIBS = -lreadline -lncurses

# Reguła domyślna
all: $(NAME)

# Kompilacja programu
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

# Kompilacja plików obiektowych
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Tworzenie folderu na pliki obiektowe
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Usuwanie plików obiektowych
clean:
	rm -f $(OBJS)

# Usuwanie plików obiektowych i programu
fclean: clean
	rm -f $(NAME)

# Rekompilacja programu
re: fclean all

# Ignorowanie plików dla make
.PHONY: all clean fclean re
