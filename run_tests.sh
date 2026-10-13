#!/bin/sh
# Usage: ./run_tests.sh [function ...]      e.g. ./run_tests.sh strlen ft_atoi
# With no arguments every function is tested.
# Env: RUNNER="valgrind ..."  wraps each test binary
#      TEST_TIMEOUT=N          seconds before a single case is killed (default 2)

cd "$(dirname "$0")" || exit 1

if [ -z "$LIBFT_DIR" ]; then
	if [ -f ../libft.h ]; then LIBFT_DIR=..; else LIBFT_DIR=../libft; fi
fi

ALL="isalpha isdigit isalnum isascii isprint strlen memset bzero memcpy
memmove strlcpy strlcat toupper tolower strchr strrchr strncmp memchr
memcmp strnstr atoi calloc strdup
substr strjoin strtrim split itoa strmapi striteri
putchar_fd putstr_fd putendl_fd putnbr_fd
lstnew lstadd_front lstsize lstlast lstadd_back lstdelone lstclear lstiter
lstmap"

[ $# -eq 0 ] && set -- $ALL

RED="$(printf '\033[31m')"
GREEN="$(printf '\033[32m')"
YELLOW="$(printf '\033[33m')"
BOLD="$(printf '\033[1m')"
RESET="$(printf '\033[0m')"

mkdir -p bin
passed=""
failed=""
broken=""

for f in "$@"; do
	f="${f#ft_}"
	if [ ! -f "tests/test_ft_$f.c" ]; then
		echo "${YELLOW}no test file for ft_$f (tests/test_ft_$f.c)${RESET}"
		broken="$broken ft_$f"
		continue
	fi
	log="bin/build_ft_$f.log"
	if ! make -s --no-print-directory "bin/test_ft_$f" >"$log" 2>&1; then
		echo "${BOLD}ft_$f${RESET}"
		echo "  ${RED}[BUILD FAIL]${RESET} could not compile/link the test against libft.a"
		if ! nm "$LIBFT_DIR/libft.a" 2>/dev/null | grep -q " T ft_$f\$"; then
			echo "      ${YELLOW}hint: libft.a does not define ft_$f (missing, misnamed, or not compiled from .c?)${RESET}"
		fi
		sed 's/^/      /' "$log" | head -n 15
		echo
		broken="$broken ft_$f"
		continue
	fi
	$RUNNER "./bin/test_ft_$f"
	status=$?
	if [ $status -eq 0 ]; then
		passed="$passed ft_$f"
	else
		[ $status -eq 42 ] && echo "  ${RED}valgrind reported memory errors above${RESET}"
		failed="$failed ft_$f"
	fi
done

count() { set -- $1; echo $#; }

echo "${BOLD}==================== SUMMARY ====================${RESET}"
echo "${GREEN}passed:     $(count "$passed")${RESET}"
[ -n "$failed" ] && echo "${RED}failed:     $(count "$failed") ->$failed${RESET}"
[ -n "$broken" ] && echo "${YELLOW}build fail: $(count "$broken") ->$broken${RESET}"

[ -z "$failed" ] && [ -z "$broken" ]
