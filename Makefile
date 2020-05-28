# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: rlucas <marvin@codam.nl>                     +#+                      #
#                                                    +#+                       #
#    Created: 2020/04/12 11:11:07 by rlucas        #+#    #+#                  #
#    Updated: 2020/05/28 10:17:01 by rlucas        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBDIR = libft/
SRCDIR = src/
OBJDIR = obj/
INCLUDES = -Iincludes/ -I$(LIBDIR)includes/

SRC =	main.c \
		read_input.c \
		handle_input.c \
		input1.c \
		exit.c \
		add_char.c \
		delete_char.c \
		move_cursor.c \
		move_cursor2.c \
		execute.c \
		builtin.c \
		tok.c \
		pipeline.c \
		vector.c \
		tables.c \
		commands.c \
		env.c \
		prompt.c \
		terminal.c \
		ryantokens.c \
		finitestatemachine.c \
		r_execute.c \
		format_toks.c \
		inputconcat.c \
		lexutils.c \
		sum_tokens.c \
		gen_tokens.c \
		utils.c

OBJ := $(addprefix $(OBJDIR), $(SRC:%.c=%.o))
SRC := $(addprefix $(SRCDIR), $(SRC))

FLAGS = -Wall -Wextra -Werror
ifdef DEBUG
FLAGS += -g -fsanitize=address
endif

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling shell..."
	@gcc $(FLAGS) $(OBJ) -o $(NAME) $(INCLUDES) \
		-L$(LIBDIR) -lft -ltermcap

compile_library:
	@echo "Compiling libft..."
	@$(MAKE) -C $(LIBDIR)

$(OBJ): compile_library
	@mkdir -p obj/
	@echo "Compiling $@"
	@gcc -c $(FLAGS) -o $@ \
		$(patsubst $(OBJDIR)%.o,$(SRCDIR)%.c,$@) \
		$(INCLUDES)

clean:
	@echo "Removing objects of libraries..."
	@$(MAKE) -C $(LIBDIR) clean
	@echo "Removing objects directory..."
	@rm -rf obj/

fclean: clean
	@echo "Removing libraries and minishell executable..."
	@$(MAKE) -C $(LIBDIR) fclean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean all re bonus compile_library
