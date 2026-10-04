#include "test.h"

/*
** `exp` is a NULL-terminated list of expected words. The result is freed
** the way the subject says (each string, then the array), so a bad
** allocation shows up as a crash/ABORT or under `make valgrind`.
*/
static void	expect(const char *s, char c, const char **exp)
{
	char	**got = ft_split(s, c);
	size_t	n_exp = 0;
	size_t	n_got = 0;

	while (exp[n_exp])
		n_exp++;
	CHECK(got != NULL, "s=\"%s\" c=%s: returned NULL", s, show(&c, 1));
	if (!got)
		return ;
	while (got[n_got])
		n_got++;
	CHECK(n_got == n_exp, "s=\"%s\" c=%s: expected %zu words, got %zu",
		s, show(&c, 1), n_exp, n_got);
	for (size_t i = 0; i < n_exp && i < n_got; i++)
		CHECK(strcmp(got[i], exp[i]) == 0,
			"s=\"%s\" c=%s: word[%zu] expected \"%s\", got %s",
			s, show(&c, 1), i, exp[i], show(got[i], strlen(got[i])));
	for (size_t i = 0; i < n_got; i++)
		free(got[i]);
	free(got);
}

#define EXPECT(s, c, ...) do { \
	const char *exp__[] = {__VA_ARGS__, NULL}; \
	expect((s), (c), exp__); \
} while (0)

#define EXPECT_EMPTY(s, c) do { \
	const char *exp__[] = {NULL}; \
	expect((s), (c), exp__); \
} while (0)

static void	basic(void)
{
	EXPECT("hello world foo", ' ', "hello", "world", "foo");
}

static void	repeated_delimiters(void)
{
	/* Consecutive delimiters do not create empty strings. */
	EXPECT("a   b    c", ' ', "a", "b", "c");
}

static void	leading_and_trailing(void)
{
	EXPECT("   hello world   ", ' ', "hello", "world");
	EXPECT(",a,b,", ',', "a", "b");
}

static void	no_delimiter(void)
{
	EXPECT("hello", ' ', "hello");
}

static void	empty_string(void)
{
	/* Returns an array holding only NULL, not NULL itself. */
	EXPECT_EMPTY("", ' ');
}

static void	only_delimiters(void)
{
	EXPECT_EMPTY("     ", ' ');
	EXPECT_EMPTY(",", ',');
}

static void	nul_delimiter(void)
{
	EXPECT("hello world", '\0', "hello world");
	EXPECT_EMPTY("", '\0');
}

static void	single_char_words(void)
{
	EXPECT("a b c d", ' ', "a", "b", "c", "d");
	EXPECT("x", ' ', "x");
}

static void	other_delimiters(void)
{
	EXPECT("one,two,,three", ',', "one", "two", "three");
	EXPECT("--a--b--", '-', "a", "b");
	EXPECT("path/to/file", '/', "path", "to", "file");
}

static void	spaces_kept_with_other_delimiter(void)
{
	EXPECT(" a , b ", ',', " a ", " b ");
}

static void	high_byte_delimiter(void)
{
	EXPECT("a\xff" "b\xff\xff" "c", '\xff', "a", "b", "c");
}

static void	words_are_separate_allocations(void)
{
	char	**got = ft_split("aa bb", ' ');

	CHECK(got != NULL, "returned NULL");
	if (!got)
		return ;
	CHECK(got[0] && got[1] && got[0] != got[1],
		"words must be separate strings");
	if (got[0] && got[1])
	{
		got[0][0] = 'X';
		CHECK_STR(got[1], "bb");
	}
	for (size_t i = 0; got[i]; i++)
		free(got[i]);
	free(got);
}

static void	many_words(void)
{
	char	s[2001];
	char	**got;
	size_t	n = 0;

	for (int i = 0; i < 1000; i++)
	{
		s[2 * i] = (char)('a' + i % 26);
		s[2 * i + 1] = ' ';
	}
	s[2000] = '\0';
	got = ft_split(s, ' ');
	CHECK(got != NULL, "returned NULL");
	if (!got)
		return ;
	while (got[n])
	{
		CHECK(got[n][0] == 'a' + (int)(n % 26) && got[n][1] == '\0',
			"word[%zu] wrong: %s", n, show(got[n], strlen(got[n])));
		free(got[n]);
		n++;
	}
	CHECK_SIZE(n, 1000);
	free(got);
}

int	main(void)
{
	printf("ft_split\n");
	run_case("basic split on ' '", basic);
	run_case("repeated delimiters -> no empty words", repeated_delimiters);
	run_case("leading and trailing delimiters", leading_and_trailing);
	run_case("no delimiter -> one word", no_delimiter);
	run_case("empty string -> { NULL } (not NULL)", empty_string);
	run_case("only delimiters -> { NULL }", only_delimiters);
	run_case("c = '\\0' -> whole string is one word", nul_delimiter);
	run_case("single-char words", single_char_words);
	run_case("other delimiters (',', '-', '/')", other_delimiters);
	run_case("spaces kept when delimiter is ','", spaces_kept_with_other_delimiter);
	run_case("non-ASCII delimiter (0xff)", high_byte_delimiter);
	run_case("each word is its own allocation", words_are_separate_allocations);
	run_case("1000 words", many_words);
	return (finish("ft_split"));
}
