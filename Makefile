# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/19 16:44:33 by ltomasze          #+#    #+#              #
#    Updated: 2025/01/25 14:04:04 by ltomasze         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -g
SRCS = src/main.c src/libft.c src/envp.c src/signals.c src/free.c src/check.c \
src/error.c src/check1.c src/sanitation.c src/sanitation1.c src/cmds.c \
src/tokenizer.c src/tokenizer1.c src/tokenizer2.c src/tokenizer3.c \
src/check_tokens.c src/cmds1.c src/cmds_redir.c src/utils.c src/builtins_exit.c \
src/builtins_export.c
#OBJS = $(SRCS:.c=.o)
OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $(NAME) -lreadline

$(OBJ_DIR)/%.o: %.c | prepare_dirs
	$(CC) $(CFLAGS) -c $< -o $@

prepare_dirs:
	mkdir -p $(OBJ_DIR)/src

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
