#include "test.h"

static unsigned int	g_indices[1024];
static char			g_chars[1024];
static size_t		g_calls;

static char	to_upper(unsigned int i, char c)
{
	(void)i;
	return ((char)toupper((unsigned char)c));
}

static char	add_index(unsigned int i, char c)
{
	return ((char)(c + (int)i));
}

static char	identity(unsigned int i, char c)
{
	(void)i;
	return (c);
}

static char	record(unsigned int i, char c)
{
	if (g_calls < 1024)
	{
		g_indices[g_calls] = i;
		g_chars[g_calls] = c;
	}
	g_calls++;
	return (c);
}

static char	always_x(unsigned int i, char c)
{
	(void)i;
	(void)c;
	return ('x');
}

static void	expect(const char *s, char (*f)(unsigned int, char),
		const char *exp)
{
	char	*got = ft_strmapi(s, f);

	CHECK(got != NULL, "s=\"%s\": returned NULL", s);
	if (!got)
		return ;
	CHECK(strcmp(got, exp) == 0, "s=\"%s\": expected \"%s\", got %s",
		s, exp, show(got, strlen(got)));
	CHECK(got != s, "returned s itself instead of a new string");
	free(got);
}

static void	uppercase(void)
{
	expect("hello, World!", to_upper, "HELLO, WORLD!");
}

static void	uses_index(void)
{
	/* 'a'+0, 'a'+1, 'a'+2, ... */
	expect("aaaaa", add_index, "abcde");
}

static void	identity_copy(void)
{
	expect("unchanged", identity, "unchanged");
}

static void	constant_function(void)
{
	expect("abc", always_x, "xxx");
}

static void	empty_string(void)
{
	g_calls = 0;
	expect("", record, "");
	CHECK(g_calls == 0, "f called %zu times on an empty string", g_calls);
}

static void	called_once_per_char_in_order(void)
{
	const char	*s = "abcdef";
	char		*got;

	g_calls = 0;
	got = ft_strmapi(s, record);
	CHECK_SIZE(g_calls, strlen(s));
	for (size_t i = 0; i < g_calls && i < strlen(s); i++)
	{
		CHECK(g_indices[i] == i, "call %zu got index %u", i, g_indices[i]);
		CHECK(g_chars[i] == s[i], "call %zu got char '%c', expected '%c'",
			i, g_chars[i], s[i]);
	}
	free(got);
}

static void	original_unchanged(void)
{
	char	s[] = "hello";
	char	*got = ft_strmapi(s, to_upper);

	CHECK_STR(s, "hello");
	free(got);
}

static void	long_string(void)
{
	size_t	n = 100000;
	char	*s = malloc(n + 1);
	char	*exp = malloc(n + 1);

	memset(s, 'q', n);
	s[n] = '\0';
	memset(exp, 'Q', n);
	exp[n] = '\0';
	expect(s, to_upper, exp);
	free(s);
	free(exp);
}

int	main(void)
{
	printf("ft_strmapi\n");
	run_case("f = toupper", uppercase);
	run_case("f uses the index (\"aaaaa\" -> \"abcde\")", uses_index);
	run_case("identity f -> same text, new string", identity_copy);
	run_case("f ignores its input", constant_function);
	run_case("empty string -> \"\", f never called", empty_string);
	run_case("f called once per char with index 0,1,2,...", called_once_per_char_in_order);
	run_case("original string is not modified", original_unchanged);
	run_case("long string (100000 chars)", long_string);
	return (finish("ft_strmapi"));
}
