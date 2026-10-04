#include "test.h"

static void	boundaries(void)
{
	const int	cs[] = {'0', '9', 'a', 'z', 'A', 'Z',
		'/', ':', '@', '[', '`', '{'};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
		CHECK(!!ft_isalnum(cs[i]) == !!isalnum(cs[i]),
			"c='%c' (%d): expected %d, got %d",
			cs[i], cs[i], !!isalnum(cs[i]), !!ft_isalnum(cs[i]));
}

static void	symbols_are_false(void)
{
	const char	*s = " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\t\n\v\f\r";

	for (size_t i = 0; s[i]; i++)
		CHECK_BOOL(ft_isalnum(s[i]), 0);
	CHECK_BOOL(ft_isalnum(0), 0);
}

static void	full_range_eof_to_255(void)
{
	for (int c = -1; c <= 255; c++)
		CHECK(!!ft_isalnum(c) == !!isalnum(c),
			"c=%d: expected %d, got %d", c, !!isalnum(c), !!ft_isalnum(c));
}

static void	out_of_range_ints(void)
{
	CHECK_BOOL(ft_isalnum('a' + 256), 0);
	CHECK_BOOL(ft_isalnum('0' + 256), 0);
	CHECK_BOOL(ft_isalnum(INT_MAX), 0);
	CHECK_BOOL(ft_isalnum(INT_MIN), 0);
}

int	main(void)
{
	printf("ft_isalnum\n");
	run_case("digit/letter boundaries and neighbours", boundaries);
	run_case("punctuation, whitespace, NUL -> false", symbols_are_false);
	run_case("every value from EOF(-1) to 255 matches libc", full_range_eof_to_255);
	run_case("out-of-range ints (a+256, INT_MAX, INT_MIN)", out_of_range_ints);
	return (finish("ft_isalnum"));
}
