#include "test.h"

static void	expect(const char *s1, const char *s2, const char *exp)
{
	char	*got = ft_strjoin(s1, s2);

	CHECK(got != NULL, "s1=\"%s\" s2=\"%s\": returned NULL", s1, s2);
	if (!got)
		return ;
	CHECK(strcmp(got, exp) == 0, "s1=\"%s\" s2=\"%s\": expected \"%s\", "
		"got %s", s1, s2, exp, show(got, strlen(got)));
	CHECK(got != s1 && got != s2, "returned an input instead of a new string");
	free(got);
}

static void	basic(void)
{
	expect("Hello, ", "world!", "Hello, world!");
	expect("a", "b", "ab");
}

static void	empty_s1(void)
{
	expect("", "world", "world");
}

static void	empty_s2(void)
{
	expect("Hello", "", "Hello");
}

static void	both_empty(void)
{
	expect("", "", "");
}

static void	high_bytes(void)
{
	expect("\xff\x80", "\x7f\x01", "\xff\x80\x7f\x01");
}

static void	same_pointer_twice(void)
{
	const char	*s = "abc";

	expect(s, s, "abcabc");
}

static void	inputs_unchanged(void)
{
	char	s1[] = "foo";
	char	s2[] = "bar";
	char	*got = ft_strjoin(s1, s2);

	CHECK_STR(s1, "foo");
	CHECK_STR(s2, "bar");
	free(got);
}

static void	long_strings(void)
{
	size_t	n = 50000;
	char	*a = malloc(n + 1);
	char	*b = malloc(n + 1);
	char	*exp = malloc(2 * n + 1);

	memset(a, 'a', n);
	a[n] = '\0';
	memset(b, 'b', n);
	b[n] = '\0';
	memcpy(exp, a, n);
	memcpy(exp + n, b, n + 1);
	expect(a, b, exp);
	free(a);
	free(b);
	free(exp);
}

int	main(void)
{
	printf("ft_strjoin\n");
	run_case("basic join", basic);
	run_case("empty s1", empty_s1);
	run_case("empty s2", empty_s2);
	run_case("both empty -> \"\" (not NULL)", both_empty);
	run_case("high / non-ASCII bytes", high_bytes);
	run_case("same string as s1 and s2", same_pointer_twice);
	run_case("s1 and s2 are not modified", inputs_unchanged);
	run_case("long strings (50000 + 50000)", long_strings);
	return (finish("ft_strjoin"));
}
