NAME = webserv
SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = obj
FILES = server ServerSocket Tcp
SRC = $(foreach item, $(FILES), $(SRC_DIR)/$(item).cpp)
OBJ = $(foreach item, $(FILES), $(OBJ_DIR)/$(item).o)
INC = -I $(INC_DIR)
CFLAGS = -Wall -Wextra -Werror
CC = c++

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ)

all:$(NAME)

clean:
	rm -f $(OBJ)
	if [ $(OBJ_DIR) != "." ]; then \
		if [ -d $(OBJ_DIR) ]; then rmdir $(OBJ_DIR); fi \
	fi

fclean: clean
	rm -f $(NAME)

re: fclean all
