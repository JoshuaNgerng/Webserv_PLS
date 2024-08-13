NAME = webserv

GCC = c++ -Wall -Wextra -Werror -g -fsanitize=address --std=c++98 -pedantic -fdiagnostics-color=always
VALGRIND = valgrind --leak-check=full \
         --show-leak-kinds=all \
         --verbose \
         --log-file=valgrind-out.txt \
         --track-origins=yes \

INC = -Iinclude -Isrc

# TEST_SRC = $(wildcard test/*.c)

# Put your src directory here
SRC_DIRS = src
SRC = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# SRC :=	$(addprefix src/, \
# 			render_image.c hit_sphere.c point_at_parameter.c parse_conf_file.c \
# 			$(addprefix hook/, \
# 				handle_key_release.c) \
# 			$(addprefix utils/, \
# 				put_pixel.c create_trgb.c render_gradient.c get_next_line.c) \
# 			$(addprefix vect_utils/, \
# 				 get.c set.c new_vect.c multi_div_sum_subtr.c multi_div_sum_subtr_d.c vec_math.c) \
# 		)

OBJDIR = obj/
OBJ := $(SRC:%.cpp=$(OBJDIR)%.o)
TEST_OBJ := $(TEST_SRC:%.c=$(OBJDIR)%.o)

all: $(NAME)

$(NAME): main.cpp $(OBJ)
	$(GCC) -o $@ $^ $(INC)
	@printf "$(GREEN)$@ is ready to run$(NC)\n"

r: $(NAME)
	@printf "$(PINK) Running $<...$(NC)\n"
	@./$<

valgrind: $(NAME)
		$(VALGRIND) ./$< scenes/more_spheres.rt


test: $(TEST_OBJ)
	@$(GCC) $^ $(INC) $(LIB) -o $@exe
	@./$@exe
	@rm $@exe

$(OBJDIR)%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	@printf "Compiling $@ \033[0K\r"
	$(GCC) -c $< $(INC) -o $@


clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re test

# Colors are great!
# Formats are greater!
# https://misc.flogisoft.com/bash/tip_colors_and_formatting
RED		= \033[31m
GREEN	= \033[32m
YELLOW	= \033[033m
BLUE	= \033[034m
PINK	= \033[035m
TEAL	= \033[036m
WHITE	= \033[037m
NC	= \033[0m # No Color