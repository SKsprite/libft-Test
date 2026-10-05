#include "test.h"

/*
** Allocation-failure injection. The Makefile links this test with
** -Wl,--wrap=malloc,--wrap=calloc,--wrap=free, so every malloc/calloc/free
** call made by libft (and this file) lands in the __wrap_ functions below,
** which forward to the real allocator (valgrind still sees every block).
** While `g_armed` is set, allocation number `g_fail_at` (0-based) returns
** NULL and every successful block is recorded in `g_live` until freed.
*/
void	*__real_malloc(size_t size);
void	*__real_calloc(size_t n, size_t size);
void	__real_free(void *p);
void	*__wrap_malloc(size_t size);
void	*__wrap_calloc(size_t n, size_t size);
void	__wrap_free(void *p);

#define MAX_TRACKED 4096

static int		g_armed = 0;
static long		g_fail_at = -1;
static long		g_nalloc = 0;
static void		*g_live[MAX_TRACKED];
static size_t	g_nlive = 0;

static void	*track(void *p)
{
	if (p && g_nlive < MAX_TRACKED)
		g_live[g_nlive++] = p;
	return (p);
}

void	*__wrap_malloc(size_t size)
{
	if (!g_armed)
		return (__real_malloc(size));
	if (g_nalloc++ == g_fail_at)
		return (NULL);
	return (track(__real_malloc(size)));
}

void	*__wrap_calloc(size_t n, size_t size)
{
	if (!g_armed)
		return (__real_calloc(n, size));
	if (g_nalloc++ == g_fail_at)
		return (NULL);
	return (track(__real_calloc(n, size)));
}

void	__wrap_free(void *p)
{
	for (size_t i = 0; p && i < g_nlive; i++)
	{
		if (g_live[i] == p)
		{
			g_live[i] = g_live[--g_nlive];
			break ;
		}
	}
	__real_free(p);
}

/* Runs ft_split with allocation `fail_at` failing (-1 = none). */
static char	**split_failing_at(const char *s, char c, long fail_at)
{
	char	**got;

	g_nlive = 0;
	g_nalloc = 0;
	g_fail_at = fail_at;
	g_armed = 1;
	got = ft_split(s, c);
	g_armed = 0;
	return (got);
}

/* Frees everything ft_split allocated (and did not free) in the last call. */
static void	release_live(void)
{
	while (g_nlive)
		free(g_live[g_nlive - 1]);
}

/* How many allocations ft_split makes for (s, c) when nothing fails. */
static long	count_allocs(const char *s, char c)
{
	char	**got = split_failing_at(s, c, -1);

	CHECK(got != NULL, "s=\"%s\" c=%s: returned NULL with no failure",
		s, show(&c, 1));
	release_live();
	return (g_nalloc);
}

/*
** Makes allocation `idx` fail: ft_split must return NULL and must have
** freed every block it allocated before the failure.
*/
static void	expect_clean_fail(const char *s, char c, long idx, long total)
{
	char	**got = split_failing_at(s, c, idx);

	CHECK(got == NULL, "s=\"%s\" c=%s: allocation %ld/%ld failed but "
		"ft_split did not return NULL", s, show(&c, 1), idx + 1, total);
	CHECK(g_nlive == 0, "s=\"%s\" c=%s: allocation %ld/%ld failed -> "
		"%zu block(s) leaked (earlier words not freed?)",
		s, show(&c, 1), idx + 1, total, g_nlive);
	release_live();
}

/* Fails each allocation of ft_split(s, c) in turn. */
static void	expect_all_fail_clean(const char *s, char c)
{
	long	total = count_allocs(s, c);

	CHECK(total > 0, "s=\"%s\": ft_split made no allocation", s);
	for (long i = 0; i < total; i++)
		expect_clean_fail(s, c, i, total);
}

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

static void	alloc_fail_first(void)
{
	/* The very first allocation (normally the array) fails. */
	expect_clean_fail("hello world foo", ' ', 0,
		count_allocs("hello world foo", ' '));
	expect_clean_fail("", ' ', 0, count_allocs("", ' '));
}

static void	alloc_fail_second(void)
{
	/* Nothing to free yet except the array itself. */
	expect_clean_fail("hello world foo", ' ', 1,
		count_allocs("hello world foo", ' '));
}

static void	alloc_fail_last(void)
{
	/* Every earlier word was allocated and must be freed. */
	long	total = count_allocs("hello world foo", ' ');

	expect_clean_fail("hello world foo", ' ', total - 1, total);
}

static void	alloc_fail_every_point(void)
{
	expect_all_fail_clean("hello world foo", ' ');
	expect_all_fail_clean("x", ' ');
	expect_all_fail_clean(",,a,,b,,c,,", ',');
	expect_all_fail_clean("one,two,,three", ',');
}

static void	alloc_fail_many_words(void)
{
	char	s[101];

	for (int i = 0; i < 50; i++)
	{
		s[2 * i] = (char)('a' + i % 26);
		s[2 * i + 1] = ' ';
	}
	s[100] = '\0';
	expect_all_fail_clean(s, ' ');
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
	run_case("alloc fail: 1st allocation -> NULL", alloc_fail_first);
	run_case("alloc fail: 2nd allocation -> NULL, no leak", alloc_fail_second);
	run_case("alloc fail: last word -> NULL, earlier words freed",
		alloc_fail_last);
	run_case("alloc fail: every allocation point -> NULL, no leak",
		alloc_fail_every_point);
	run_case("alloc fail: every point over 50 words", alloc_fail_many_words);
	return (finish("ft_split"));
}
