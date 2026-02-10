# Name of your final executable
NAME        := compiler

# Directories
SRC_DIR     := src
OBJ_DIR     := obj

# Source files (relative to SRC_DIR)
# We manually list them to ensure utils_test.c is ignored
SRCS        := main.c \
               utils/utils_error.c \
               utils/utils_file.c \
               utils/utils_token.c \
			   utils/utils_general.c \
			   utils/utils_parser.c \
               lexer/read_file.c \
			   lexer/lexer.c \
			   lexer/handlers.c \
			   

# Convert source files to object files in the OBJ_DIR
OBJS        := $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Compiler and Flags
CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -Iinc -Isrc/utils -Isrcs/lexer

# Build Rules
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Successfully built $(NAME)"

# Rule to compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	@rm -f $(NAME)
	@echo "Executable removed."

re: fclean all

.PHONY: all clean fclean re
