# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/13 16:47:15 by jngerng           #+#    #+#              #
#    Updated: 2024/09/05 15:41:42 by jngerng          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = obj
FILES = Const CheckFile Parse ParsingError Socket Server ServerBlock Client Location
SRC = $(foreach item, $(FILES), $(SRC_DIR)/$(item).cpp)
M_SRC = main
OBJ = $(foreach item, $(FILES) $(M_SRC), $(OBJ_DIR)/$(item).o)
INC = -I $(INC_DIR)
CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic
CC = c++

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INC) -o $(NAME) $(OBJ)

all:$(NAME)

clean:
	rm -f $(OBJ)
	if [ -d $(OBJ_DIR) ]; then rmdir $(OBJ_DIR); fi

fclean: clean
	rm -f $(NAME)

re: fclean all
