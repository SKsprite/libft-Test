# Default: the tester is cloned inside the libft repo, so libft is the parent
# directory. Falls back to a sibling ../libft when the parent has no libft.h.
LIBFT_DIR	?= $(if $(wildcard ../libft.h),..,../libft)
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
PART3		= lstnew lstadd_front lstsize lstlast lstadd_back \
			  lstdelone lstclear lstiter lstmap
FUNCS		= $(PART1) $(PART2) $(PART3)

BIN_DIR		= bin
UTILS_OBJ	= $(BIN_DIR)/test_utils.o
LST_OBJ		= $(BIN_DIR)/lst_utils.o
LST_BINS	= $(addprefix $(BIN_DIR)/test_ft_, $(PART3))
BINS		= $(addprefix $(BIN_DIR)/test_ft_, $(FUNCS))

export LIBFT_DIR

VALGRIND	= RUNNER="valgrind -q --leak-check=full --errors-for-leak-kinds=definite \
			  --error-exitcode=42" TEST_TIMEOUT=30

.PHONY: all test part1 part2 part3 build libft valgrind valgrind_part1 \
		valgrind_part2 valgrind_part3 \
		clean fclean re remove $(FUNCS) $(addprefix ft_, $(FUNCS))

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

# Linked list functions (lstnew ... lstmap).
part3: libft
	@./run_tests.sh $(PART3)

# Run the suite under valgrind (leaks, invalid reads/writes).
valgrind: libft
	@$(VALGRIND) ./run_tests.sh $(FUNCS)

valgrind_part1: libft
	@$(VALGRIND) ./run_tests.sh $(PART1)

valgrind_part2: libft
	@$(VALGRIND) ./run_tests.sh $(PART2)

valgrind_part3: libft
	@$(VALGRIND) ./run_tests.sh $(PART3)

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

# ft_split's tests make chosen allocations fail (see tests/test_ft_split.c).
$(BIN_DIR)/test_ft_split: LDLIBS += -Wl,--wrap=malloc,--wrap=calloc,--wrap=free

$(LST_OBJ): lst_utils.c lst_utils.h test.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# The linked list tests track malloc/free and inject failures (lst_utils.c).
$(LST_BINS): EXTRA_OBJ = $(LST_OBJ)
$(LST_BINS): LDLIBS += -Wl,--wrap=malloc,--wrap=calloc,--wrap=free
$(LST_BINS): $(LST_OBJ) lst_utils.h

$(BIN_DIR)/test_ft_%: tests/test_ft_%.c $(UTILS_OBJ) test.h $(LIBFT) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(UTILS_OBJ) $(EXTRA_OBJ) -o $@ $(LDLIBS)

clean:
	rm -rf $(BIN_DIR)

fclean: clean

re: fclean all

# Deletes this whole tester directory (e.g. before submitting libft). Refuses
# unless the directory really is the tester, so it can't wipe anything else.
remove:
	@test -f "$(CURDIR)/run_tests.sh" && test -f "$(CURDIR)/test.h" \
		|| { echo "refusing: $(CURDIR) does not look like the tester"; exit 1; }
	@echo "Removing $(CURDIR)"
	@rm -rf -- "$(CURDIR)"
