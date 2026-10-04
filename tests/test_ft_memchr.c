#include "test.h"

static void	compare(const char *s, int c, size_t n)
{
	void	*got = ft_memchr(s, c, n);
	void	*exp = memchr(s, c, n);

	CHECK(got == exp, "s=%s c=%d n=%zu: expected %s, got %s",
		show(s, n), c, n,
		exp ? show(exp, 1) : "NULL", got ? show(got, 1) : "NULL");
	if (got != exp && got && exp)
		printf("         (offset expected %td, got %td)\n",
			(const char *)exp - s, (const char *)got - s);
}

static void	found(void)
{
	compare("Hello, world!", 'o', 13);
	compare("Hello, world!", 'H', 13);
	compare("Hello, world!", '!', 13);
}

static void	not_found(void)
{
	compare("Hello, world!", 'z', 13);
}

static void	respects_n(void)
{
	compare("Hello, world!", 'w', 5);
	compare("Hello, world!", 'o', 4);
	compare("Hello, world!", 'o', 5);
}

static void	n_zero(void)
{
	compare("abc", 'a', 0);
}

static void	finds_nul(void)
{
	compare("abc", '\0', 4);
}

static void	searches_past_nul(void)
{
	/* memchr does not stop at '\0' like strchr does. */
	compare("ab\0cd", 'c', 5);
	compare("\0\0\0x", 'x', 4);
}

static void	c_converted_to_unsigned_char(void)
{
	compare("ab\xff" "cd", -1, 5);
	compare("ab\xff" "cd", 255, 5);
	compare("abcd", 'c' + 256, 4);
	compare("\x80", 0x80, 1);
}

static void	every_byte_value(void)
{
	unsigned char	buf[256];

	for (int i = 0; i < 256; i++)
		buf[i] = (unsigned char)(255 - i);
	for (int c = 0; c < 256; c++)
		compare((const char *)buf, c, sizeof(buf));
}

int	main(void)
{
	printf("ft_memchr\n");
	run_case("found at start, middle, end", found);
	run_case("not found -> NULL", not_found);
	run_case("only searches the first n bytes", respects_n);
	run_case("n = 0 -> NULL", n_zero);
	run_case("can find '\\0'", finds_nul);
	run_case("keeps searching past '\\0'", searches_past_nul);
	run_case("c converted to unsigned char (-1, 255, 'c'+256)", c_converted_to_unsigned_char);
	run_case("every byte value 0..255", every_byte_value);
	return (finish("ft_memchr"));
}
