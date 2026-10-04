#include "test.h"

static void	expect(char *s)
{
	int		fd = capture_open();
	size_t	len;
	size_t	exp_len = strlen(s) + 1;
	char	*exp = malloc(exp_len + 1);
	char	*out;

	memcpy(exp, s, exp_len - 1);
	exp[exp_len - 1] = '\n';
	exp[exp_len] = '\0';
	ft_putendl_fd(s, fd);
	out = capture_read(fd, &len);
	CHECK(len == exp_len && memcmp(out, exp, len) == 0,
		"expected %zu bytes %s, got %zu bytes %s",
		exp_len, show(exp, exp_len), len, show(out, len));
	free(exp);
	free(out);
}

static void	basic(void)
{
	expect("Hello, world!");
}

static void	empty_string(void)
{
	/* Still prints the newline. */
	expect("");
}

static void	already_has_newline(void)
{
	/* A second '\n' is still added. */
	expect("line\n");
}

static void	stops_at_nul(void)
{
	char	s[] = "abc\0def";

	expect(s);
}

static void	high_bytes(void)
{
	expect("\xff\x80\x7f");
}

static void	long_string(void)
{
	size_t	n = 100000;
	char	*s = malloc(n + 1);

	memset(s, 'z', n);
	s[n] = '\0';
	expect(s);
	free(s);
}

static void	several_calls(void)
{
	int		fd = capture_open();
	size_t	len;
	char	*out;

	ft_putendl_fd("one", fd);
	ft_putendl_fd("", fd);
	ft_putendl_fd("two", fd);
	out = capture_read(fd, &len);
	CHECK(len == 9 && memcmp(out, "one\n\ntwo\n", 9) == 0,
		"expected \"one\\n\\ntwo\\n\", got %s", show(out, len));
	free(out);
}

static void	invalid_fd_no_crash(void)
{
	ft_putendl_fd("hello", -1);
}

int	main(void)
{
	printf("ft_putendl_fd\n");
	run_case("string followed by '\\n'", basic);
	run_case("empty string -> just \"\\n\"", empty_string);
	run_case("string ending in '\\n' gets another one", already_has_newline);
	run_case("stops at embedded '\\0'", stops_at_nul);
	run_case("non-ASCII bytes", high_bytes);
	run_case("long string (100000 chars)", long_string);
	run_case("several calls, one line each", several_calls);
	run_case("fd = -1 does not crash", invalid_fd_no_crash);
	return (finish("ft_putendl_fd"));
}
