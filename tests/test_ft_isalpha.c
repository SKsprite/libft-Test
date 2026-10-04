#include "test.h"

static void	boundaries(void)
{
	const int	cs[] = {'a', 'z', 'A', 'Z', '@', '[', '`', '{', 'm', 'M'};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
		CHECK(!!ft_isalpha(cs[i]) == !!isalpha(cs[i]),
			"c='%c' (%d): expected %d, got %d",
			cs[i], cs[i], !!isalpha(cs[i]), !!ft_isalpha(cs[i]));
}

static void	digits_and_symbols_are_false(void)
{
	const char	*s = "0123456789 !\"#$%&'()*+,-./:;<=>?\\]^_|}~\t\n";

	for (size_t i = 0; s[i]; i++)
		CHECK_BOOL(ft_isalpha(s[i]), 0);
}

static void	full_range_eof_to_255(void)
{
	for (int c = -1; c <= 255; c++)
		CHECK(!!ft_isalpha(c) == !!isalpha(c),
			"c=%d: expected %d, got %d", c, !!isalpha(c), !!ft_isalpha(c));
}

static void	out_of_range_ints(void)
{
	/* Values whose low byte is a letter must still be rejected. */
	CHECK_BOOL(ft_isalpha('a' + 256), 0);
	CHECK_BOOL(ft_isalpha('A' - 256), 0);
	CHECK_BOOL(ft_isalpha(INT_MAX), 0);
	CHECK_BOOL(ft_isalpha(INT_MIN), 0);
}

int	main(void)
{
	printf("ft_isalpha\n");
	run_case("letter boundaries a/z/A/Z and neighbours", boundaries);
	run_case("digits, punctuation, whitespace -> false", digits_and_symbols_are_false);
	run_case("every value from EOF(-1) to 255 matches libc", full_range_eof_to_255);
	run_case("out-of-range ints (a+256, INT_MAX, INT_MIN)", out_of_range_ints);
	return (finish("ft_isalpha"));
}
