#include "test.h"

static void	expect(const char *s, unsigned int start, size_t len,
		const char *exp)
{
	char	*got = ft_substr(s, start, len);

	CHECK(got != NULL, "s=\"%s\" start=%u len=%zu: returned NULL, expected "
		"\"%s\"", s, start, len, exp);
	if (!got)
		return ;
	CHECK(strcmp(got, exp) == 0, "s=\"%s\" start=%u len=%zu: expected "
		"\"%s\", got %s", s, start, len, exp, show(got, strlen(got)));
	CHECK(got != s, "returned s itself instead of a new allocation");
	free(got);
}

static void	middle(void)
{
	expect("Hello, world!", 7, 5, "world");
	expect("Hello, world!", 2, 3, "llo");
}

static void	from_start(void)
{
	expect("Hello, world!", 0, 5, "Hello");
	expect("Hello", 0, 5, "Hello");
}

static void	len_zero(void)
{
	expect("Hello", 0, 0, "");
	expect("Hello", 3, 0, "");
}

static void	len_past_end(void)
{
	/* Only the characters that exist are copied. */
	expect("Hello, world!", 7, 100, "world!");
	expect("Hello", 0, 6, "Hello");
	expect("Hello", 4, 2, "o");
}

static void	len_size_max(void)
{
	/* Must not try to malloc SIZE_MAX bytes. */
	expect("Hello", 1, SIZE_MAX, "ello");
}

static void	start_at_end(void)
{
	expect("Hello", 5, 3, "");
}

static void	start_past_end(void)
{
	/* Must not read past the string: returns an empty string. */
	expect("Hello", 6, 3, "");
	expect("Hello", 100, 3, "");
	expect("Hello", UINT_MAX, 3, "");
	expect("", 1, 1, "");
}

static void	empty_string(void)
{
	expect("", 0, 0, "");
	expect("", 0, 10, "");
}

static void	last_char(void)
{
	expect("Hello", 4, 1, "o");
}

static void	result_is_independent(void)
{
	char	s[] = "abcdef";
	char	*got = ft_substr(s, 1, 3);

	CHECK(got != NULL, "returned NULL");
	if (!got)
		return ;
	s[2] = 'X';
	CHECK_STR(got, "bcd");
	free(got);
}

int	main(void)
{
	printf("ft_substr\n");
	run_case("substring in the middle", middle);
	run_case("substring from index 0", from_start);
	run_case("len = 0 -> \"\"", len_zero);
	run_case("len goes past the end -> clipped", len_past_end);
	run_case("len = SIZE_MAX (no huge malloc)", len_size_max);
	run_case("start == strlen(s) -> \"\"", start_at_end);
	run_case("start > strlen(s) -> \"\" (incl. UINT_MAX)", start_past_end);
	run_case("empty s", empty_string);
	run_case("only the last char", last_char);
	run_case("result is a separate copy", result_is_independent);
	return (finish("ft_substr"));
}
