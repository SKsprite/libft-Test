#include "test.h"

static void	boundaries(void)
{
	const int	cs[] = {'0', '9', '/', ':', '5'};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
		CHECK(!!ft_isdigit(cs[i]) == !!isdigit(cs[i]),
			"c='%c' (%d): expected %d, got %d",
			cs[i], cs[i], !!isdigit(cs[i]), !!ft_isdigit(cs[i]));
}

static void	letters_are_false(void)
{
	CHECK_BOOL(ft_isdigit('a'), 0);
	CHECK_BOOL(ft_isdigit('Z'), 0);
	CHECK_BOOL(ft_isdigit(' '), 0);
	CHECK_BOOL(ft_isdigit(0), 0);
}

static void	full_range_eof_to_255(void)
{
	for (int c = -1; c <= 255; c++)
		CHECK(!!ft_isdigit(c) == !!isdigit(c),
			"c=%d: expected %d, got %d", c, !!isdigit(c), !!ft_isdigit(c));
}

static void	out_of_range_ints(void)
{
	CHECK_BOOL(ft_isdigit('0' + 256), 0);
	CHECK_BOOL(ft_isdigit('5' - 256), 0);
	CHECK_BOOL(ft_isdigit(INT_MAX), 0);
	CHECK_BOOL(ft_isdigit(INT_MIN), 0);
}

int	main(void)
{
	printf("ft_isdigit\n");
	run_case("digit boundaries 0/9 and neighbours / :", boundaries);
	run_case("letters, space, NUL -> false", letters_are_false);
	run_case("every value from EOF(-1) to 255 matches libc", full_range_eof_to_255);
	run_case("out-of-range ints ('0'+256, INT_MAX, INT_MIN)", out_of_range_ints);
	return (finish("ft_isdigit"));
}
