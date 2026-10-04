#include "test.h"

static void	compare(const void *a, const void *b, size_t n)
{
	int	got = ft_memcmp(a, b, n);
	int	exp = memcmp(a, b, n);

	CHECK(SIGN(got) == SIGN(exp), "s1=%s s2=%s n=%zu: expected sign %d (%d), "
		"got sign %d (%d)", show(a, n), show(b, n), n,
		SIGN(exp), exp, SIGN(got), got);
}

static void	equal(void)
{
	compare("abcdef", "abcdef", 6);
	compare("abcdef", "abcdef", 7);
}

static void	different(void)
{
	compare("abcdef", "abcxef", 6);
	compare("abcxef", "abcdef", 6);
	compare("a", "b", 1);
	compare("b", "a", 1);
}

static void	n_zero(void)
{
	compare("abc", "xyz", 0);
}

static void	difference_beyond_n(void)
{
	compare("abcdef", "abcxyz", 3);
}

static void	does_not_stop_at_nul(void)
{
	/* Bytes after '\0' still count. */
	compare("ab\0c", "ab\0d", 4);
	compare("ab\0d", "ab\0c", 4);
	compare("\0\0\0", "\0\0\1", 3);
}

static void	unsigned_comparison(void)
{
	/* Bytes must be compared as unsigned char: 0x80 > 0x00. */
	compare("\x80", "\x00", 1);
	compare("\x00", "\x80", 1);
	compare("\xff", "a", 1);
	compare("a\xfe", "a\x01", 2);
}

static void	last_byte_differs(void)
{
	compare("abcdefgh", "abcdefgi", 8);
}

static void	int_arrays(void)
{
	int	a[] = {1, 2, 3, 4};
	int	b[] = {1, 2, 3, 5};

	compare(a, b, sizeof(a));
	compare(a, a, sizeof(a));
}

int	main(void)
{
	printf("ft_memcmp\n");
	run_case("equal buffers -> 0", equal);
	run_case("different buffers -> correct sign", different);
	run_case("n = 0 -> 0", n_zero);
	run_case("difference beyond n ignored", difference_beyond_n);
	run_case("does not stop at '\\0'", does_not_stop_at_nul);
	run_case("compares as unsigned char (0x80 vs 0x00, 0xff vs 'a')", unsigned_comparison);
	run_case("only the last byte differs", last_byte_differs);
	run_case("int arrays", int_arrays);
	return (finish("ft_memcmp"));
}
