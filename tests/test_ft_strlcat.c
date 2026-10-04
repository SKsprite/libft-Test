#include "test.h"

#define BUF 32

/*
** dst starts as `init` followed by 'X' filler, so writes past the
** terminator and missing NULs are both visible in the buffer compare.
*/
static void	compare(const char *init, const char *src, size_t size)
{
	char	got[BUF];
	char	exp[BUF];
	size_t	rg;
	size_t	re;

	memset(got, 'X', BUF);
	memset(exp, 'X', BUF);
	memcpy(got, init, strlen(init) + 1);
	memcpy(exp, init, strlen(init) + 1);
	rg = ft_strlcat(got, src, size);
	re = strlcat(exp, src, size);
	CHECK(rg == re, "dst=\"%s\" src=\"%s\" size=%zu: return expected %zu, "
		"got %zu", init, src, size, re, rg);
	CHECK(memcmp(got, exp, BUF) == 0, "dst=\"%s\" src=\"%s\" size=%zu:\n"
		"         expected %s\n         got      %s",
		init, src, size, show(exp, BUF), show(got, BUF));
}

static void	fits(void)
{
	compare("Hello", ", world", 20);
}

static void	size_zero(void)
{
	compare("Hello", "abc", 0);
}

static void	size_less_than_dst_len(void)
{
	/* Returns size + strlen(src); dst untouched. */
	compare("Hello", "abc", 3);
	compare("Hello", "abc", 1);
}

static void	size_equals_dst_len(void)
{
	compare("Hello", "abc", 5);
}

static void	size_equals_dst_len_plus_one(void)
{
	/* Room only for the terminator: nothing appended. */
	compare("Hello", "abc", 6);
}

static void	partial_append(void)
{
	compare("Hello", ", world", 8);
	compare("Hello", ", world", 12);
}

static void	exact_fit(void)
{
	compare("Hello", ", world", 13);
}

static void	empty_dst(void)
{
	compare("", "abc", 10);
	compare("", "abc", 2);
	compare("", "abc", 0);
}

static void	empty_src(void)
{
	compare("Hello", "", 10);
	compare("Hello", "", 3);
}

static void	both_empty(void)
{
	compare("", "", 0);
	compare("", "", 1);
}

static void	dst_not_terminated_within_size(void)
{
	/* No '\0' in the first `size` bytes of dst: return size + strlen(src). */
	char	got[BUF];
	char	exp[BUF];
	size_t	rg;
	size_t	re;

	memset(got, 'X', BUF);
	memset(exp, 'X', BUF);
	got[BUF - 1] = '\0';
	exp[BUF - 1] = '\0';
	rg = ft_strlcat(got, "abc", 10);
	re = strlcat(exp, "abc", 10);
	CHECK_SIZE(rg, re);
	CHECK(memcmp(got, exp, BUF) == 0, "expected %s\n         got      %s",
		show(exp, BUF), show(got, BUF));
}

int	main(void)
{
	printf("ft_strlcat\n");
	run_case("src fits completely", fits);
	run_case("size = 0", size_zero);
	run_case("size < strlen(dst): untouched, returns size + strlen(src)", size_less_than_dst_len);
	run_case("size == strlen(dst)", size_equals_dst_len);
	run_case("size == strlen(dst) + 1: nothing appended", size_equals_dst_len_plus_one);
	run_case("partial append, NUL-terminated", partial_append);
	run_case("exact fit", exact_fit);
	run_case("empty dst", empty_dst);
	run_case("empty src", empty_src);
	run_case("both empty", both_empty);
	run_case("dst not NUL-terminated within size", dst_not_terminated_within_size);
	return (finish("ft_strlcat"));
}
