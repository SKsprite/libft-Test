#include "test.h"

static void	boundaries(void)
{
	const int	cs[] = {' ', '~', 31, 127, 'a', '0'};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
		CHECK(!!ft_isprint(cs[i]) == !!isprint(cs[i]),
			"c=%d: expected %d, got %d",
			cs[i], !!isprint(cs[i]), !!ft_isprint(cs[i]));
}

static void	whitespace_controls_are_false(void)
{
	/* \t \n \v \f \r are whitespace but NOT printable. */
	CHECK_BOOL(ft_isprint('\t'), 0);
	CHECK_BOOL(ft_isprint('\n'), 0);
	CHECK_BOOL(ft_isprint('\v'), 0);
	CHECK_BOOL(ft_isprint('\f'), 0);
	CHECK_BOOL(ft_isprint('\r'), 0);
	CHECK_BOOL(ft_isprint(0), 0);
}

static void	full_range_eof_to_255(void)
{
	for (int c = -1; c <= 255; c++)
		CHECK(!!ft_isprint(c) == !!isprint(c),
			"c=%d: expected %d, got %d", c, !!isprint(c), !!ft_isprint(c));
}

static void	out_of_range_ints(void)
{
	CHECK_BOOL(ft_isprint('a' + 256), 0);
	CHECK_BOOL(ft_isprint(' ' - 256), 0);
	CHECK_BOOL(ft_isprint(INT_MAX), 0);
	CHECK_BOOL(ft_isprint(INT_MIN), 0);
}

int	main(void)
{
	printf("ft_isprint\n");
	run_case("boundaries ' ' (32), '~' (126), 31, 127 (DEL)", boundaries);
	run_case("\\t \\n \\v \\f \\r and NUL -> false", whitespace_controls_are_false);
	run_case("every value from EOF(-1) to 255 matches libc", full_range_eof_to_255);
	run_case("out-of-range ints (a+256, INT_MAX, INT_MIN)", out_of_range_ints);
	return (finish("ft_isprint"));
}
