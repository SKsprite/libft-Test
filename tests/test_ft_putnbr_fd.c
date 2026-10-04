#include "test.h"

/* The expected output comes from snprintf("%d"). */
static void	expect(int n)
{
	int		fd = capture_open();
	char	exp[16];
	size_t	exp_len;
	size_t	len;
	char	*out;

	exp_len = (size_t)snprintf(exp, sizeof(exp), "%d", n);
	ft_putnbr_fd(n, fd);
	out = capture_read(fd, &len);
	CHECK(len == exp_len && memcmp(out, exp, len) == 0,
		"n=%d: expected \"%s\", got %s", n, exp, show(out, len));
	free(out);
}

static void	zero(void)
{
	expect(0);
}

static void	single_digits(void)
{
	for (int n = -9; n <= 9; n++)
		expect(n);
}

static void	positive(void)
{
	expect(42);
	expect(12345);
	expect(1000000);
}

static void	negative(void)
{
	expect(-42);
	expect(-12345);
	expect(-1000000);
}

static void	powers_of_ten(void)
{
	for (int p = 1; ; p *= 10)
	{
		expect(p);
		expect(p - 1);
		expect(-p);
		if (p == 1000000000)
			break ;
	}
}

static void	int_max(void)
{
	expect(INT_MAX);
}

static void	int_min(void)
{
	/* -INT_MIN overflows an int: must be handled specially. */
	expect(INT_MIN);
	expect(INT_MIN + 1);
}

static void	range_sweep(void)
{
	for (int n = -2000; n <= 2000; n++)
		expect(n);
}

static void	several_calls(void)
{
	int		fd = capture_open();
	size_t	len;
	char	*out;

	ft_putnbr_fd(1, fd);
	ft_putnbr_fd(-23, fd);
	ft_putnbr_fd(0, fd);
	out = capture_read(fd, &len);
	CHECK(len == 5 && memcmp(out, "1-230", 5) == 0,
		"expected \"1-230\", got %s", show(out, len));
	free(out);
}

static void	invalid_fd_no_crash(void)
{
	ft_putnbr_fd(INT_MIN, -1);
	ft_putnbr_fd(42, -1);
}

int	main(void)
{
	printf("ft_putnbr_fd\n");
	run_case("0", zero);
	run_case("single digits -9..9", single_digits);
	run_case("positive numbers", positive);
	run_case("negative numbers", negative);
	run_case("powers of ten and their neighbours", powers_of_ten);
	run_case("INT_MAX", int_max);
	run_case("INT_MIN (-2147483648)", int_min);
	run_case("sweep -2000..2000", range_sweep);
	run_case("several calls append (no separators)", several_calls);
	run_case("fd = -1 does not crash", invalid_fd_no_crash);
	return (finish("ft_putnbr_fd"));
}
