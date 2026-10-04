#include "test.h"

static void	lowercase_converted(void)
{
	for (int c = 'a'; c <= 'z'; c++)
		CHECK(ft_toupper(c) == toupper(c),
			"c='%c': expected '%c', got %d", c, toupper(c), ft_toupper(c));
}

static void	others_unchanged(void)
{
	const int	cs[] = {'A', 'Z', '@', '[', '`', '{', '0', ' ', 0, 127};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
		CHECK_INT(ft_toupper(cs[i]), toupper(cs[i]));
}

static void	full_range_eof_to_255(void)
{
	for (int c = -1; c <= 255; c++)
		CHECK(ft_toupper(c) == toupper(c),
			"c=%d: expected %d, got %d", c, toupper(c), ft_toupper(c));
}

static void	out_of_range_ints_unchanged(void)
{
	CHECK_INT(ft_toupper('a' + 256), 'a' + 256);
	CHECK_INT(ft_toupper(INT_MAX), INT_MAX);
	CHECK_INT(ft_toupper(INT_MIN), INT_MIN);
}

int	main(void)
{
	printf("ft_toupper\n");
	run_case("'a'..'z' converted to 'A'..'Z'", lowercase_converted);
	run_case("uppercase, '`', '{', digits, NUL unchanged", others_unchanged);
	run_case("every value from EOF(-1) to 255 matches libc", full_range_eof_to_255);
	run_case("out-of-range ints returned unchanged", out_of_range_ints_unchanged);
	return (finish("ft_toupper"));
}
