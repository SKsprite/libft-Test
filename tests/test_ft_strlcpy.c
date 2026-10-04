#include "test.h"

#define BUF 32

/* Destination is pre-filled with 'X' so out-of-bounds writes are visible. */
static void	compare(const char *src, size_t size)
{
	char	got[BUF];
	char	exp[BUF];
	size_t	rg;
	size_t	re;

	memset(got, 'X', BUF);
	memset(exp, 'X', BUF);
	rg = ft_strlcpy(got, src, size);
	re = strlcpy(exp, src, size);
	CHECK(rg == re, "src=\"%s\" size=%zu: return expected %zu, got %zu",
		src, size, re, rg);
	CHECK(memcmp(got, exp, BUF) == 0,
		"src=\"%s\" size=%zu:\n         expected %s\n         got      %s",
		src, size, show(exp, BUF), show(got, BUF));
}

static void	fits(void)
{
	compare("Hello", 20);
}

static void	size_zero(void)
{
	/* Nothing written (not even '\0'), returns strlen(src). */
	compare("Hello", 0);
}

static void	size_one(void)
{
	compare("Hello", 1);
}

static void	truncation(void)
{
	compare("Hello, world!", 5);
	compare("Hello, world!", 2);
}

static void	size_equals_len(void)
{
	compare("Hello", 5);
}

static void	size_equals_len_plus_one(void)
{
	compare("Hello", 6);
}

static void	empty_src(void)
{
	compare("", 0);
	compare("", 1);
	compare("", 10);
}

static void	return_is_full_src_len(void)
{
	const char	*src = "this source string is definitely longer than BUF";
	char		dst[4];

	CHECK_SIZE(ft_strlcpy(dst, src, sizeof(dst)), strlen(src));
	CHECK_STR(dst, "thi");
}

static void	high_bytes(void)
{
	compare("\xff\x80\x7f", 10);
}

int	main(void)
{
	printf("ft_strlcpy\n");
	run_case("src fits in dst", fits);
	run_case("size = 0: writes nothing, returns strlen(src)", size_zero);
	run_case("size = 1: dst becomes \"\"", size_one);
	run_case("truncation, always NUL-terminated", truncation);
	run_case("size == strlen(src) (truncates last char)", size_equals_len);
	run_case("size == strlen(src) + 1 (exact fit)", size_equals_len_plus_one);
	run_case("empty src with size 0, 1, 10", empty_src);
	run_case("return value is strlen(src) even when truncated", return_is_full_src_len);
	run_case("high / non-ASCII bytes", high_bytes);
	return (finish("ft_strlcpy"));
}
