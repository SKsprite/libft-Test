#include "test.h"

static void	expect_char(char c)
{
	int		fd = capture_open();
	size_t	len;
	char	*out;

	ft_putchar_fd(c, fd);
	out = capture_read(fd, &len);
	CHECK(len == 1 && out[0] == c, "c=%s: expected exactly 1 byte %s, "
		"got %zu byte(s) %s", show(&c, 1), show(&c, 1), len, show(out, len));
	free(out);
}

static void	letter(void)
{
	expect_char('a');
	expect_char('Z');
}

static void	newline(void)
{
	expect_char('\n');
}

static void	nul_byte(void)
{
	/* '\0' is still one byte to write. */
	expect_char('\0');
}

static void	high_bytes(void)
{
	expect_char('\xff');
	expect_char('\x80');
}

static void	every_byte(void)
{
	for (int c = 0; c < 256; c++)
		expect_char((char)c);
}

static void	several_calls_append(void)
{
	int		fd = capture_open();
	size_t	len;
	char	*out;

	ft_putchar_fd('4', fd);
	ft_putchar_fd('2', fd);
	ft_putchar_fd('!', fd);
	out = capture_read(fd, &len);
	CHECK(len == 3 && memcmp(out, "42!", 3) == 0,
		"expected \"42!\", got %s", show(out, len));
	free(out);
}

static void	invalid_fd_no_crash(void)
{
	ft_putchar_fd('a', -1);
}

int	main(void)
{
	printf("ft_putchar_fd\n");
	run_case("letters", letter);
	run_case("'\\n'", newline);
	run_case("'\\0' writes one zero byte", nul_byte);
	run_case("non-ASCII bytes (0xff, 0x80)", high_bytes);
	run_case("every byte value 0..255", every_byte);
	run_case("several calls append in order", several_calls_append);
	run_case("fd = -1 does not crash", invalid_fd_no_crash);
	return (finish("ft_putchar_fd"));
}
