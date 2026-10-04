#include "test.h"

static void	expect(const char *s1, const char *set, const char *exp)
{
	char	*got = ft_strtrim(s1, set);

	CHECK(got != NULL, "s1=%s set=%s: returned NULL",
		show(s1, strlen(s1)), show(set, strlen(set)));
	if (!got)
		return ;
	CHECK(strcmp(got, exp) == 0, "s1=%s set=%s: expected %s, got %s",
		show(s1, strlen(s1)), show(set, strlen(set)),
		show(exp, strlen(exp)), show(got, strlen(got)));
	CHECK(got != s1, "returned s1 itself instead of a new string");
	free(got);
}

static void	spaces(void)
{
	expect("   hello   ", " ", "hello");
}

static void	multi_char_set(void)
{
	expect("xyxhelloyx", "xy", "hello");
	expect("xyxhelloyx", "yx", "hello");
}

static void	middle_untouched(void)
{
	/* Only the ends are trimmed. */
	expect("a b a", "a", " b ");
	expect("--a-b-c--", "-", "a-b-c");
}

static void	only_one_side(void)
{
	expect("   hello", " ", "hello");
	expect("hello   ", " ", "hello");
}

static void	everything_trimmed(void)
{
	expect("aaaa", "a", "");
	expect("abcabc", "cba", "");
}

static void	nothing_to_trim(void)
{
	expect("hello", " ", "hello");
	expect("hello", "xyz", "hello");
}

static void	empty_set(void)
{
	expect("  hello  ", "", "  hello  ");
}

static void	empty_s1(void)
{
	expect("", " ", "");
	expect("", "", "");
}

static void	whitespace_set(void)
{
	expect("\t\n  hello world \n\t", " \n\t", "hello world");
}

static void	single_char_left(void)
{
	expect("  x  ", " ", "x");
}

static void	set_char_repeated(void)
{
	expect("aaahelloaaa", "aaaa", "hello");
}

int	main(void)
{
	printf("ft_strtrim\n");
	run_case("trim spaces on both sides", spaces);
	run_case("set with several chars (order does not matter)", multi_char_set);
	run_case("chars in the middle are kept", middle_untouched);
	run_case("trim only at start / only at end", only_one_side);
	run_case("every char is in set -> \"\" (not NULL)", everything_trimmed);
	run_case("nothing to trim -> copy", nothing_to_trim);
	run_case("empty set -> copy", empty_set);
	run_case("empty s1 -> \"\"", empty_s1);
	run_case("whitespace set \" \\n\\t\"", whitespace_set);
	run_case("one char left after trimming", single_char_left);
	run_case("set with repeated chars", set_char_repeated);
	return (finish("ft_strtrim"));
}
