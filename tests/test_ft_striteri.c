#include "test.h"

static unsigned int	g_indices[1024];
static char			*g_ptrs[1024];
static size_t		g_calls;

static void	to_upper(unsigned int i, char *c)
{
	(void)i;
	*c = (char)toupper((unsigned char)*c);
}

static void	add_index(unsigned int i, char *c)
{
	*c = (char)(*c + (int)i);
}

static void	record(unsigned int i, char *c)
{
	if (g_calls < 1024)
	{
		g_indices[g_calls] = i;
		g_ptrs[g_calls] = c;
	}
	g_calls++;
}

static void	modifies_in_place(void)
{
	char	s[] = "hello, World!";

	ft_striteri(s, to_upper);
	CHECK_STR(s, "HELLO, WORLD!");
}

static void	uses_index(void)
{
	char	s[] = "aaaaa";

	ft_striteri(s, add_index);
	CHECK_STR(s, "abcde");
}

static void	passes_address_inside_s(void)
{
	/* f must get &s[i] (not a pointer to a copy). */
	char	s[] = "abcdef";

	g_calls = 0;
	ft_striteri(s, record);
	CHECK_SIZE(g_calls, strlen(s));
	for (size_t i = 0; i < g_calls && i < strlen(s); i++)
	{
		CHECK(g_indices[i] == i, "call %zu got index %u", i, g_indices[i]);
		CHECK(g_ptrs[i] == &s[i], "call %zu got pointer s+%td, expected s+%zu",
			i, g_ptrs[i] - s, i);
	}
}

static void	empty_string(void)
{
	char	s[] = "";

	g_calls = 0;
	ft_striteri(s, record);
	CHECK(g_calls == 0, "f called %zu times on an empty string", g_calls);
	CHECK_INT(s[0], '\0');
}

static void	stops_at_terminator(void)
{
	char	s[8] = "abc\0xyz";

	ft_striteri(s, to_upper);
	CHECK_MEM(s, "ABC\0xyz", 8);
}

int	main(void)
{
	printf("ft_striteri\n");
	run_case("modifies s in place (toupper)", modifies_in_place);
	run_case("f uses the index (\"aaaaa\" -> \"abcde\")", uses_index);
	run_case("f gets index i and &s[i], once per char", passes_address_inside_s);
	run_case("empty string -> f never called", empty_string);
	run_case("stops at '\\0' (bytes after are untouched)", stops_at_terminator);
	return (finish("ft_striteri"));
}
