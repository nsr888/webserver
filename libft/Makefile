# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anasyrov <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/29 15:48:54 by anasyrov          #+#    #+#              #
#    Updated: 2020/05/13 13:22:39 by ksinistr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC_LST = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c \
		  ft_lstdelone.c ft_lstlast.c ft_lstnew.c ft_lstsize.c \
		  ft_lstiter.c ft_lstmap.c
SOURCES = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
		  ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memccpy.c \
		  ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
		  ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
		  ft_split.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlcat.c \
		  ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
		  ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c \
		  ft_tolower.c ft_toupper.c 
INCLUDES = libft.h
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
OBJ_LST := $(patsubst %.c, %.o, $(SRC_LST))
CFLAGS = -Wall -Werror -Wextra

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(OBJECTS): %.o: %.c $(INCLUDES)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(OBJ_LST)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

bonus: all $(OBJ_LST)
	ar rs $(NAME) $(OBJ_LST)

$(OBJ_LST): %.o: %.c $(INCLUDES)
	gcc $(CFLAGS) -c $< -o $@
