# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/19 16:44:33 by ltomasze          #+#    #+#              #
#    Updated: 2024/12/22 17:06:10 by ltomasze         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -g
SRCS = src/main.c src/libft.c src/envp.c src/signals.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $(NAME) -lreadline

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
