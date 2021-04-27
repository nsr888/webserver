NAME = webserv
SRC = main.cpp Client.cpp WebServer.cpp EventLoop.cpp Request.cpp Response.cpp \
	  Setting.cpp Parser.cpp utils.cpp ProcessMethod.cpp
OBJ = $(SRC:%.cpp=%.o)
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -g -O2 -std=c++98 -pedantic
INC = -I.

all: $(NAME)

$(NAME): $(OBJ)
	cd libft && make
	cd libgnl && make
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) -Llibft -lft -Llibgnl -lgnl -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -Ilibft -Ignl -c $< -o $@
	
clean:
	cd libft && make $@
	cd libgnl && make $@
	rm -rf $(OBJ)

fclean: clean
	cd libft && make $@
	cd libgnl && make $@
	rm -rf $(NAME)

re: fclean all

# python unit tests with pytest
test: $(NAME)
	@./$(NAME) & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python -m pytest --durations=0
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

# echo webserver response to cmd
echo: $(NAME)
	@./$(NAME) & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python3 echo_server.py
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

.PONY: all fclean clean re test echo
