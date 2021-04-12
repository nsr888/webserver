NAME = libft.a
SOURCES = ft_atoi.c ft_isdigit.c
INCLUDES = libft.h
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
CFLAGS = -Wall -Werror -Wextra

.PHONY: all clean fclean re

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
