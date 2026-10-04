#include "test.h"

static void	boundaries(void)
{
	CHECK_BOOL(ft_isascii(0), isascii(0));
	CHECK_BOOL(ft_isascii(127), isascii(127));
	CHECK_BOOL(ft_isascii(128), isascii(128));
	CHECK_BOOL(ft_isascii(-1), isascii(-1));
}

static void	full_range_eof_to_255(void)
{
	for (int c = -1; c <= 255; c++)
		CHECK(!!ft_isascii(c) == !!isascii(c),
			"c=%d: expected %d, got %d", c, !!isascii(c), !!ft_isascii(c));
}

static void	large_and_negative_ints(void)
{
	/* isascii() is defined for every int value, so libc is the oracle here. */
	const int	cs[] = {-128, -129, 256, 'a' + 256, 1000, INT_MAX, INT_MIN};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
		CHECK(!!ft_isascii(cs[i]) == !!isascii(cs[i]),
			"c=%d: expected %d, got %d",
			cs[i], !!isascii(cs[i]), !!ft_isascii(cs[i]));
}

int	main(void)
{
	printf("ft_isascii\n");
	run_case("boundaries 0, 127, 128, -1", boundaries);
	run_case("every value from EOF(-1) to 255 matches libc", full_range_eof_to_255);
	run_case("large / negative ints (256, INT_MAX, INT_MIN, ...)", large_and_negative_ints);
	return (finish("ft_isascii"));
}
