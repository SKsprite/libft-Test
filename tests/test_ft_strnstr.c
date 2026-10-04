#include "test.h"

static void	compare(const char *big, const char *little, size_t len)
{
	char	*got = ft_strnstr(big, little, len);
	char	*exp = strnstr(big, little, len);

	CHECK(got == exp, "big=\"%s\" little=\"%s\" len=%zu: expected %s, got %s",
		big, little, len,
		exp ? show(exp, strlen(exp)) : "NULL",
		got ? show(got, strlen(got)) : "NULL");
}

static void	found(void)
{
	compare("Hello, world!", "world", 13);
	compare("Hello, world!", "Hello", 13);
	compare("Hello, world!", "!", 13);
}

static void	not_found(void)
{
	compare("Hello, world!", "planet", 13);
	compare("Hello, world!", "worlds", 13);
}

static void	empty_little(void)
{
	/* Empty needle returns big, whatever len is. */
	compare("Hello", "", 5);
	compare("Hello", "", 0);
	compare("", "", 0);
}

static void	empty_big(void)
{
	compare("", "a", 5);
	compare("", "a", 0);
}

static void	len_cuts_match(void)
{
	compare("Hello, world!", "world", 11);
	compare("Hello, world!", "world", 12);
	compare("Hello, world!", "world", 10);
}

static void	len_zero(void)
{
	compare("Hello", "H", 0);
}

static void	len_larger_than_big(void)
{
	/* Must stop at the end of big. */
	compare("Hello", "lo", 100);
	compare("Hello", "lox", 100);
	compare("Hello", "o", SIZE_MAX);
}

static void	little_longer_than_big(void)
{
	compare("abc", "abcd", 10);
}

static void	partial_match_then_real_match(void)
{
	compare("aaab", "aab", 4);
	compare("abababc", "ababc", 7);
	compare("mississippi", "issip", 11);
}

static void	match_at_exact_len_boundary(void)
{
	compare("abcdef", "def", 6);
	compare("abcdef", "def", 5);
	compare("abcdef", "a", 1);
}

int	main(void)
{
	printf("ft_strnstr\n");
	run_case("found at start, middle, end", found);
	run_case("not found -> NULL", not_found);
	run_case("empty little -> big", empty_little);
	run_case("empty big", empty_big);
	run_case("len cuts the match short", len_cuts_match);
	run_case("len = 0", len_zero);
	run_case("len larger than big (incl. SIZE_MAX)", len_larger_than_big);
	run_case("little longer than big", little_longer_than_big);
	run_case("partial match followed by real match", partial_match_then_real_match);
	run_case("match ending exactly at len", match_at_exact_len_boundary);
	return (finish("ft_strnstr"));
}
