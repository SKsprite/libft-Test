#include "test.h"

static void	empty_string(void)
{
	CHECK_SIZE(ft_strlen(""), strlen(""));
}

static void	single_char(void)
{
	CHECK_SIZE(ft_strlen("a"), strlen("a"));
}

static void	regular_strings(void)
{
	const char	*ss[] = {"Hello, world!", "42", "  spaces  ", "\t\n"};

	for (size_t i = 0; i < sizeof(ss) / sizeof(*ss); i++)
		CHECK(ft_strlen(ss[i]) == strlen(ss[i]), "s=%s: expected %zu, got %zu",
			show(ss[i], strlen(ss[i])), strlen(ss[i]), ft_strlen(ss[i]));
}

static void	stops_at_embedded_nul(void)
{
	CHECK_SIZE(ft_strlen("abc\0def"), 3);
}

static void	high_bytes(void)
{
	const char	*s = "\xff\x80\x7f" "abc";

	CHECK_SIZE(ft_strlen(s), strlen(s));
}

static void	long_string(void)
{
	size_t	n = 100000;
	char	*s = malloc(n + 1);

	memset(s, 'x', n);
	s[n] = '\0';
	CHECK_SIZE(ft_strlen(s), strlen(s));
	free(s);
}

int	main(void)
{
	printf("ft_strlen\n");
	run_case("empty string -> 0", empty_string);
	run_case("single char -> 1", single_char);
	run_case("regular strings", regular_strings);
	run_case("stops at embedded NUL (\"abc\\0def\" -> 3)", stops_at_embedded_nul);
	run_case("non-ASCII / high bytes are counted", high_bytes);
	run_case("long string (100000 chars)", long_string);
	return (finish("ft_strlen"));
}
