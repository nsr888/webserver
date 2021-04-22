NAME = libgnl.a
SOURCES = get_next_line.c get_next_line_utils.c
INCLUDES = get_next_line.h
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
CFLAGS = -Wall -Werror -Wextra

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(OBJECTS): %.o: %.c $(INCLUDES)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
