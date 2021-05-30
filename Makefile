NAME = webserv
SRC = main.cpp Client.cpp WebServer.cpp EventLoop.cpp Request.cpp Response.cpp \
	  Setting.cpp Parser.cpp utils.cpp ProcessMethod.cpp
OBJ = $(SRC:%.cpp=%.o)
DEPENDS := $(patsubst %.cpp,%.d,$(SRC))
CXX = g++
# CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -g -O2 -std=c++98 -pedantic
INC = -I.

all: $(NAME)

$(NAME): $(OBJ)
	cd libft && make
	cd libgnl && make
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) -Llibft -lft -Llibgnl -lgnl -lstdc++ -o $(NAME)

-include $(DEPENDS)

%.o: %.cpp %.hpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -Ilibft -Ignl -c $< -o $@
	
clean:
	cd libft && make $@
	cd libgnl && make $@
	rm -rf $(OBJ)
	rm -rf $(DEPENDS)

fclean: clean
	cd libft && make $@
	cd libgnl && make $@
	rm -rf $(NAME)

re: fclean all

# test with arthur config
ta: $(NAME)
	@./$(NAME) config/default.conf & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python3 -m pytest --tb=line || true
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

# test with igor config
ti: $(NAME)
	@./$(NAME) config/defaultIgor.conf & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python3 -m pytest --tb=line || true
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

# test with igor config
td: $(NAME)
	@./$(NAME) config/defaultDenis.conf & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python3 -m pytest --tb=line || true
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

# python unit tests with pytest
test: $(NAME)
	@./$(NAME) & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python3 -m pytest --tb=line || true
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

# python unit tests with pytest (default) 'long' tracebacks
test_long: $(NAME)
	@./$(NAME) & echo $$! > ./tests/webserver.PID || true
	@cd ./tests/ && python3 -m pytest --durations=0
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

.PONY: all fclean clean re test echo ta ti td
