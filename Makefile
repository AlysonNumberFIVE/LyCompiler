NAME         := compiler
TEST_NAME    := parser_test

# Directories
SRC_DIR      := src
OBJ_DIR      := obj

# 1. SHARED SOURCES (Everything EXCEPT the files with main())
SHARED_SRCS  := utils/utils_error.c \
                utils/utils_file.c \
                utils/utils_token.c \
                utils/utils_general.c \
                utils/utils_parser.c \
                lexer/read_file.c \
                lexer/lexer.c \
                lexer/handlers.c \
                parser/parser.c \
                parser/parse_parameters.c \
				parser/parse_statements.c \
				parser/parse_expression.c \
				parser/parser_print.c \
				parser/parse_function_call.c \
				parser/parse_if_statement.c \
				parser/parse_while_statement.c
                # parser/errors.c

# 2. ENTRY POINTS
MAIN_SRC     := main.c
TEST_SRC     := parser/parse_statement_test.c

# 3. COMPOSE FULL SOURCE LISTS
# For "make all"
SRCS         := $(MAIN_SRC) $(SHARED_SRCS)
# For "make parser"
PARSER_TEST_SRCS := $(TEST_SRC) $(SHARED_SRCS)

# Convert to object files
OBJS              := $(SRCS:%.c=$(OBJ_DIR)/%.o)
PARSER_TEST_OBJS  := $(PARSER_TEST_SRCS:%.c=$(OBJ_DIR)/%.o)

# Compiler and Flags
CC           := gcc
CFLAGS       := -Wall -Wextra -Werror -Iinc -Isrc/utils -Isrc/lexer -Isrc/parser

# Build Rules
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Successfully built $(NAME)"

# This target is totally independent of "make all"
parser: $(PARSER_TEST_OBJS)
	@$(CC) $(CFLAGS) $(PARSER_TEST_OBJS) -o $(TEST_NAME)
	@echo "Successfully built $(TEST_NAME)"
	@echo "Running Parser Tests..."
	@./$(TEST_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	@rm -f $(NAME) $(TEST_NAME)
	@echo "Executables removed."

re: fclean all

.PHONY: all clean fclean re parser