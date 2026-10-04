#include "test.h"

static void	compare(const char *a, const char *b, size_t n)
{
	int	got = ft_strncmp(a, b, n);
	int	exp = strncmp(a, b, n);

	CHECK(SIGN(got) == SIGN(exp), "s1=%s s2=%s n=%zu: expected sign %d (%d), "
		"got sign %d (%d)", show(a, strlen(a)), show(b, strlen(b)), n,
		SIGN(exp), exp, SIGN(got), got);
}

static void	equal(void)
{
	compare("abcdef", "abcdef", 6);
	compare("abcdef", "abcdef", 3);
}

static void	different(void)
{
	compare("abcdef", "abcxef", 6);
	compare("abcxef", "abcdef", 6);
	compare("a", "b", 1);
}

static void	n_zero(void)
{
	compare("abc", "xyz", 0);
}

static void	difference_beyond_n(void)
{
	compare("abcdef", "abcxyz", 3);
}

static void	n_larger_than_strings(void)
{
	/* Must stop at '\0' and not read past the strings. */
	compare("abc", "abc", 100);
	compare("abc", "abd", 100);
	compare("", "", 100);
}

static void	stops_at_nul(void)
{
	compare("test\0abc", "test\0xyz", 10);
}

static void	prefix(void)
{
	compare("abc", "abcd", 4);
	compare("abcd", "abc", 4);
	compare("abc", "abcd", 3);
	compare("", "a", 1);
	compare("a", "", 1);
}

static void	unsigned_comparison(void)
{
	/* Chars must be compared as unsigned char: '\200' > '\0'. */
	compare("\200", "\0", 1);
	compare("\0", "\200", 1);
	compare("\xff", "a", 1);
	compare("a\xfe", "a\x01", 2);
	compare("test\200", "test\0", 6);
}

static void	size_max(void)
{
	compare("abc", "abd", SIZE_MAX);
	compare("abc", "abc", SIZE_MAX);
}

int	main(void)
{
	printf("ft_strncmp\n");
	run_case("equal strings -> 0", equal);
	run_case("different strings -> correct sign", different);
	run_case("n = 0 -> 0", n_zero);
	run_case("difference beyond n ignored", difference_beyond_n);
	run_case("n larger than strings (stops at '\\0')", n_larger_than_strings);
	run_case("content after '\\0' ignored", stops_at_nul);
	run_case("one string a prefix of the other", prefix);
	run_case("compares as unsigned char ('\\200' vs '\\0')", unsigned_comparison);
	run_case("n = SIZE_MAX", size_max);
	return (finish("ft_strncmp"));
}
