# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/27 15:59:42 by jngerng           #+#    #+#              #
#    Updated: 2024/08/27 16:06:11 by jngerng          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test
SRC_DIR = .
INC_DIR = .
OBJ_DIR = obj
FILES = resetfd simple_fd_array
SRC = $(foreach item, $(FILES), $(SRC_DIR)/$(item).cpp)
OBJ = $(foreach item, $(FILES), $(OBJ_DIR)/$(item).o)
INC = -I $(INC_DIR)
CFLAGS = -Wall -Wextra -Werror -std=c++98 #-pedantic
CC = c++

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
