NAME = webserver
SRC = 	main.cpp \
		Client.cpp \
		WebServer.cpp
OBJ = $(SRC:%.cpp=%.o)
CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror -g -O2
INC = -I.

all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

test: 
	@./$(NAME) & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python -m pytest
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

debug: 
	@./$(NAME) & echo $$! > ./tests/webserver.PID
	@cd ./tests/ && python3 debug_server.py
	@if [ -a ./tests/webserver.PID ]; then \
		kill -TERM $$(cat ./tests/webserver.PID) || true; \
	fi;
	@rm -rf ./tests/webserver.PID

.PONY: all fclean clean re test
