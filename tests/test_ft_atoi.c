#include "test.h"

static void	compare_all(const char **ss, size_t count)
{
	for (size_t i = 0; i < count; i++)
		CHECK(ft_atoi(ss[i]) == atoi(ss[i]), "s=%s: expected %d, got %d",
			show(ss[i], strlen(ss[i])), atoi(ss[i]), ft_atoi(ss[i]));
}

#define COMPARE(...) do { \
	const char *ss__[] = {__VA_ARGS__}; \
	compare_all(ss__, sizeof(ss__) / sizeof(*ss__)); \
} while (0)

static void	simple_numbers(void)
{
	COMPARE("0", "1", "42", "123456", "9");
}

static void	signs(void)
{
	COMPARE("-42", "+42", "-0", "+0", "-1");
}

static void	invalid_signs(void)
{
	/* Only one sign is allowed, and it must touch the digits. */
	COMPARE("--42", "++42", "+-42", "-+42", "- 42", "+ 42", "-", "+");
}

static void	leading_whitespace(void)
{
	COMPARE("   42", "\t42", "\n42", "\v42", "\f42", "\r42",
		" \t\n\v\f\r -42", "   +7");
}

static void	non_whitespace_prefix(void)
{
	/* \x7f, \b and letters are NOT whitespace. */
	COMPARE("\x7f" "42", "\b42", "a42", "_42", ".42");
}

static void	trailing_garbage(void)
{
	COMPARE("42abc", "42 43", "-42-", "1.5", "12\t3", "\n\n\n  -46\b9 \n5d6");
}

static void	no_digits(void)
{
	COMPARE("", "abc", "   ", " - ");
}

static void	leading_zeros(void)
{
	COMPARE("0000123", "-0000123", "+000", "00000000000000000042");
}

static void	int_limits(void)
{
	COMPARE("2147483647", "-2147483648", "  -2147483648abc", "+2147483647");
}

int	main(void)
{
	printf("ft_atoi\n");
	run_case("simple numbers", simple_numbers);
	run_case("single sign: -42, +42, -0, +0", signs);
	run_case("multiple / detached signs -> 0", invalid_signs);
	run_case("leading whitespace (space \\t \\n \\v \\f \\r)", leading_whitespace);
	run_case("non-whitespace prefix -> 0", non_whitespace_prefix);
	run_case("stops at first non-digit", trailing_garbage);
	run_case("no digits -> 0 (\"\", \"abc\", \"   \")", no_digits);
	run_case("leading zeros", leading_zeros);
	run_case("INT_MAX and INT_MIN", int_limits);
	return (finish("ft_atoi"));
}
