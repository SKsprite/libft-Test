#include "test.h"

static void	expect(char *s)
{
	int		fd = capture_open();
	size_t	len;
	size_t	exp_len = strlen(s);
	char	*out;

	ft_putstr_fd(s, fd);
	out = capture_read(fd, &len);
	CHECK(len == exp_len && memcmp(out, s, len) == 0,
		"expected %zu bytes %s, got %zu bytes %s",
		exp_len, show(s, exp_len), len, show(out, len));
	free(out);
}

static void	basic(void)
{
	expect("Hello, world!");
}

static void	empty_string(void)
{
	expect("");
}

static void	single_char(void)
{
	expect("x");
}

static void	no_extra_newline(void)
{
	expect("no newline added");
	expect("ends with newline\n");
}

static void	stops_at_nul(void)
{
	char	s[] = "abc\0def";

	expect(s);
}

static void	high_bytes(void)
{
	expect("\xff\x80\x7f\x01");
}

static void	long_string(void)
{
	size_t	n = 100000;
	char	*s = malloc(n + 1);

	for (size_t i = 0; i < n; i++)
		s[i] = (char)('a' + i % 26);
	s[n] = '\0';
	expect(s);
	free(s);
}

static void	several_calls_append(void)
{
	int		fd = capture_open();
	size_t	len;
	char	*out;

	ft_putstr_fd("foo", fd);
	ft_putstr_fd("", fd);
	ft_putstr_fd("bar", fd);
	out = capture_read(fd, &len);
	CHECK(len == 6 && memcmp(out, "foobar", 6) == 0,
		"expected \"foobar\", got %s", show(out, len));
	free(out);
}

static void	invalid_fd_no_crash(void)
{
	ft_putstr_fd("hello", -1);
}

int	main(void)
{
	printf("ft_putstr_fd\n");
	run_case("basic string", basic);
	run_case("empty string writes nothing", empty_string);
	run_case("single char", single_char);
	run_case("no newline is added", no_extra_newline);
	run_case("stops at embedded '\\0'", stops_at_nul);
	run_case("non-ASCII bytes", high_bytes);
	run_case("long string (100000 chars)", long_string);
	run_case("several calls append in order", several_calls_append);
	run_case("fd = -1 does not crash", invalid_fd_no_crash);
	return (finish("ft_putstr_fd"));
}
