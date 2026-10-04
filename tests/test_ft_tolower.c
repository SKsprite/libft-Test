#include "test.h"

static void	uppercase_converted(void)
{
	for (int c = 'A'; c <= 'Z'; c++)
		CHECK(ft_tolower(c) == tolower(c),
			"c='%c': expected '%c', got %d", c, tolower(c), ft_tolower(c));
}

static void	others_unchanged(void)
{
	const int	cs[] = {'a', 'z', '@', '[', '`', '{', '0', ' ', 0, 127};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
		CHECK_INT(ft_tolower(cs[i]), tolower(cs[i]));
}

static void	full_range_eof_to_255(void)
{
	for (int c = -1; c <= 255; c++)
		CHECK(ft_tolower(c) == tolower(c),
			"c=%d: expected %d, got %d", c, tolower(c), ft_tolower(c));
}

static void	out_of_range_ints_unchanged(void)
{
	CHECK_INT(ft_tolower('A' + 256), 'A' + 256);
	CHECK_INT(ft_tolower(INT_MAX), INT_MAX);
	CHECK_INT(ft_tolower(INT_MIN), INT_MIN);
}

int	main(void)
{
	printf("ft_tolower\n");
	run_case("'A'..'Z' converted to 'a'..'z'", uppercase_converted);
	run_case("lowercase, '@', '[', digits, NUL unchanged", others_unchanged);
	run_case("every value from EOF(-1) to 255 matches libc", full_range_eof_to_255);
	run_case("out-of-range ints returned unchanged", out_of_range_ints_unchanged);
	return (finish("ft_tolower"));
}
