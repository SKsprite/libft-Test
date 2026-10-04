#include "test.h"

#define BUF 32

static void	compare(const char *init, size_t init_len, int c, size_t n)
{
	unsigned char	got[BUF];
	unsigned char	exp[BUF];
	void			*ret;

	memset(got, 'X', BUF);
	memset(exp, 'X', BUF);
	memcpy(got, init, init_len);
	memcpy(exp, init, init_len);
	ret = ft_memset(got, c, n);
	memset(exp, c, n);
	CHECK_PTR(ret, got);
	CHECK(memcmp(got, exp, BUF) == 0, "c=%d n=%zu:\n         expected %s\n"
		"         got      %s", c, n, show(exp, BUF), show(got, BUF));
}

static void	basic(void)
{
	compare("", 0, 'A', 10);
}

static void	returns_s(void)
{
	char	buf[4];

	CHECK_PTR(ft_memset(buf, 0, sizeof(buf)), buf);
}

static void	n_zero_writes_nothing(void)
{
	compare("abc", 3, 'Z', 0);
}

static void	whole_buffer(void)
{
	compare("", 0, '*', BUF);
}

static void	buffer_full_of_nuls(void)
{
	/* memset must not stop at '\0' bytes in the destination. */
	char	zeros[BUF] = {0};

	compare(zeros, BUF, 'B', 16);
}

static void	c_converted_to_unsigned_char(void)
{
	compare("", 0, 'A' + 256, 8);
	compare("", 0, -1, 8);
	compare("", 0, 0x1234, 8);
	compare("hello", 5, 0, 3);
}

static void	int_array(void)
{
	int	got[8];
	int	exp[8];

	ft_memset(got, 0xff, sizeof(got));
	memset(exp, 0xff, sizeof(exp));
	CHECK_MEM(got, exp, sizeof(got));
}

static void	large_buffer(void)
{
	size_t	n = 1 << 20;
	char	*got = malloc(n);
	char	*exp = malloc(n);

	memset(got, 0, n);
	memset(exp, 0, n);
	ft_memset(got, 'q', n - 1);
	memset(exp, 'q', n - 1);
	CHECK_MEM(got, exp, n);
	free(got);
	free(exp);
}

int	main(void)
{
	printf("ft_memset\n");
	run_case("basic fill, bytes after n untouched", basic);
	run_case("returns s", returns_s);
	run_case("n = 0 writes nothing", n_zero_writes_nothing);
	run_case("fill entire buffer", whole_buffer);
	run_case("destination full of '\\0' (must not stop at NUL)", buffer_full_of_nuls);
	run_case("c converted to unsigned char ('A'+256, -1, 0x1234, 0)", c_converted_to_unsigned_char);
	run_case("int array with 0xff", int_array);
	run_case("large buffer (1 MiB)", large_buffer);
	return (finish("ft_memset"));
}
