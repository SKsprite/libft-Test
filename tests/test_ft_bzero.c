#include "test.h"

#define BUF 32

static void	compare(const char *init, size_t init_len, size_t n)
{
	unsigned char	got[BUF];
	unsigned char	exp[BUF];

	memset(got, 'X', BUF);
	memset(exp, 'X', BUF);
	memcpy(got, init, init_len);
	memcpy(exp, init, init_len);
	ft_bzero(got, n);
	bzero(exp, n);
	CHECK(memcmp(got, exp, BUF) == 0, "n=%zu:\n         expected %s\n"
		"         got      %s", n, show(exp, BUF), show(got, BUF));
}

static void	basic(void)
{
	compare("Hello, world!", 13, 5);
}

static void	n_zero_writes_nothing(void)
{
	compare("abc", 3, 0);
}

static void	whole_buffer(void)
{
	compare("abcdefghijklmnopqrstuvwxyz", 26, BUF);
}

static void	does_not_stop_at_nul(void)
{
	/* A '\0' in the middle of the region must not end the zeroing. */
	compare("ab\0cdefgh", 9, 9);
}

static void	does_not_stop_at_nul_first_byte(void)
{
	compare("\0bcdefgh", 8, 8);
}

static void	large_buffer(void)
{
	size_t	n = 1 << 20;
	char	*got = malloc(n);
	char	*exp = malloc(n);

	memset(got, 'z', n);
	memset(exp, 'z', n);
	ft_bzero(got, n - 1);
	bzero(exp, n - 1);
	CHECK_MEM(got, exp, n);
	free(got);
	free(exp);
}

int	main(void)
{
	printf("ft_bzero\n");
	run_case("zero first 5 bytes, rest untouched", basic);
	run_case("n = 0 writes nothing", n_zero_writes_nothing);
	run_case("zero entire buffer", whole_buffer);
	run_case("embedded '\\0' does not stop zeroing", does_not_stop_at_nul);
	run_case("first byte already '\\0' does not stop zeroing", does_not_stop_at_nul_first_byte);
	run_case("large buffer (1 MiB)", large_buffer);
	return (finish("ft_bzero"));
}
