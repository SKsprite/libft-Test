#include "test.h"

#define BUF 32

static void	compare(const void *src, size_t n)
{
	unsigned char	got[BUF];
	unsigned char	exp[BUF];
	void			*ret;

	memset(got, 'X', BUF);
	memset(exp, 'X', BUF);
	ret = ft_memcpy(got, src, n);
	memcpy(exp, src, n);
	CHECK_PTR(ret, got);
	CHECK(memcmp(got, exp, BUF) == 0, "n=%zu:\n         expected %s\n"
		"         got      %s", n, show(exp, BUF), show(got, BUF));
}

static void	basic(void)
{
	compare("Hello, world!", 14);
}

static void	partial_copy(void)
{
	compare("Hello, world!", 5);
}

static void	n_zero(void)
{
	compare("abc", 0);
}

static void	copies_past_nul(void)
{
	/* memcpy copies raw bytes; '\0' must not stop it. */
	compare("ab\0cd\0ef", 8);
}

static void	high_bytes(void)
{
	compare("\xff\x80\x00\x7f\x01", 5);
}

static void	returns_dest(void)
{
	char	dst[8];

	CHECK_PTR(ft_memcpy(dst, "abc", 4), dst);
}

static void	null_null_zero(void)
{
	/* With n == 0 nothing is accessed; libc returns dest (NULL). */
	CHECK_PTR(ft_memcpy(opaque_ptr(NULL), opaque_ptr(NULL), 0), NULL);
}

static void	struct_copy(void)
{
	struct s_pt { int x; long y; char z[5]; }	a = {42, -1L, "abcd"};
	struct s_pt									b;

	memset(&b, 0, sizeof(b));
	ft_memcpy(&b, &a, sizeof(a));
	CHECK_MEM(&b, &a, sizeof(a));
}

static void	large_buffer(void)
{
	size_t	n = 1 << 20;
	char	*src = malloc(n);
	char	*got = calloc(n, 1);

	for (size_t i = 0; i < n; i++)
		src[i] = (char)(i * 7);
	ft_memcpy(got, src, n);
	CHECK_MEM(got, src, n);
	free(src);
	free(got);
}

int	main(void)
{
	printf("ft_memcpy\n");
	run_case("basic copy incl. terminator", basic);
	run_case("partial copy, bytes after n untouched", partial_copy);
	run_case("n = 0 copies nothing", n_zero);
	run_case("copies past embedded '\\0'", copies_past_nul);
	run_case("high / non-ASCII bytes", high_bytes);
	run_case("returns dest", returns_dest);
	run_case("dest = src = NULL with n = 0 -> NULL, no crash", null_null_zero);
	run_case("struct copy", struct_copy);
	run_case("large buffer (1 MiB)", large_buffer);
	return (finish("ft_memcpy"));
}
