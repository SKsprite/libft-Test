#include "test.h"

static void	compare(const char *s)
{
	char	*got = ft_strdup(s);

	CHECK(got != NULL, "ft_strdup(%s) returned NULL", show(s, strlen(s)));
	if (!got)
		return ;
	CHECK(got != s, "returned the same pointer instead of a copy");
	CHECK(strcmp(got, s) == 0, "expected %s, got %s",
		show(s, strlen(s)), show(got, strlen(got)));
	free(got);
}

static void	basic(void)
{
	compare("Hello, world!");
}

static void	empty_string(void)
{
	compare("");
}

static void	single_char(void)
{
	compare("x");
}

static void	high_bytes(void)
{
	compare("\xff\x80\x7f" "abc");
}

static void	stops_at_embedded_nul(void)
{
	char	*got = ft_strdup("abc\0def");

	CHECK(got != NULL, "returned NULL");
	if (got)
		CHECK_STR(got, "abc");
	free(got);
}

static void	copy_is_independent(void)
{
	char	src[] = "original";
	char	*got = ft_strdup(src);

	CHECK(got != NULL, "returned NULL");
	if (!got)
		return ;
	got[0] = 'X';
	src[1] = 'Y';
	CHECK_STR(got, "Xriginal");
	CHECK_STR(src, "oYiginal");
	free(got);
}

static void	long_string(void)
{
	size_t	n = 100000;
	char	*s = malloc(n + 1);

	for (size_t i = 0; i < n; i++)
		s[i] = (char)('a' + i % 26);
	s[n] = '\0';
	compare(s);
	free(s);
}

static void	many_dups(void)
{
	char	buf[64];

	for (int i = 0; i < 64; i++)
	{
		memset(buf, 'a' + i % 26, (size_t)i);
		buf[i] = '\0';
		compare(buf);
	}
}

int	main(void)
{
	printf("ft_strdup\n");
	run_case("basic copy, new pointer", basic);
	run_case("empty string -> \"\" (not NULL)", empty_string);
	run_case("single char", single_char);
	run_case("high / non-ASCII bytes", high_bytes);
	run_case("stops at embedded '\\0'", stops_at_embedded_nul);
	run_case("copy is independent of the original", copy_is_independent);
	run_case("long string (100000 chars)", long_string);
	run_case("lengths 0..63, every result free()-able", many_dups);
	return (finish("ft_strdup"));
}
