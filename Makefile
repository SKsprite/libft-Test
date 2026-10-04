LIBFT_DIR	?= ../libft
LIBFT		= $(LIBFT_DIR)/libft.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g -I. -I$(LIBFT_DIR)
LDLIBS		= -L$(LIBFT_DIR) -lft -lbsd

PART1		= isalpha isdigit isalnum isascii isprint \
			  strlen memset bzero memcpy memmove strlcpy strlcat \
			  toupper tolower strchr strrchr strncmp memchr memcmp \
			  strnstr atoi calloc strdup
PART2		= substr strjoin strtrim split itoa strmapi striteri \
			  putchar_fd putstr_fd putendl_fd putnbr_fd
FUNCS		= $(PART1) $(PART2)

BIN_DIR		= bin
UTILS_OBJ	= $(BIN_DIR)/test_utils.o
BINS		= $(addprefix $(BIN_DIR)/test_ft_, $(FUNCS))

export LIBFT_DIR

VALGRIND	= RUNNER="valgrind -q --leak-check=full --errors-for-leak-kinds=definite \
			  --error-exitcode=42" TEST_TIMEOUT=30

.PHONY: all test part1 part2 build libft valgrind valgrind_part1 valgrind_part2 \
		clean fclean re $(FUNCS) $(addprefix ft_, $(FUNCS))

all: test

# Build libft, then build + run every test (one function at a time).
test: libft
	@./run_tests.sh $(FUNCS)

# Libc functions (isalpha ... strdup).
part1: libft
	@./run_tests.sh $(PART1)

# Additional functions (substr ... putnbr_fd).
part2: libft
	@./run_tests.sh $(PART2)

# Run the suite under valgrind (leaks, invalid reads/writes).
valgrind: libft
	@$(VALGRIND) ./run_tests.sh $(FUNCS)

valgrind_part1: libft
	@$(VALGRIND) ./run_tests.sh $(PART1)

valgrind_part2: libft
	@$(VALGRIND) ./run_tests.sh $(PART2)

# `make strlen` or `make ft_strlen` runs a single function's tests.
$(FUNCS): libft
	@./run_tests.sh $@

$(addprefix ft_, $(FUNCS)): libft
	@./run_tests.sh $(@:ft_%=%)

build: libft $(BINS)

libft:
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(BIN_DIR):
	@mkdir -p $@

$(UTILS_OBJ): test_utils.c test.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/test_ft_%: tests/test_ft_%.c $(UTILS_OBJ) test.h $(LIBFT) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(UTILS_OBJ) -o $@ $(LDLIBS)

clean:
	rm -rf $(BIN_DIR)

fclean: clean

re: fclean all
