#include "test.h"

#define BUF 32

/* Run ft_memmove and memmove on identical buffers with the same offsets. */
static void	compare(size_t dst_off, size_t src_off, size_t n)
{
	char	got[BUF + 1] = "0123456789abcdefghijklmnopqrstuv";
	char	exp[BUF + 1] = "0123456789abcdefghijklmnopqrstuv";
	void	*ret;

	ret = ft_memmove(got + dst_off, got + src_off, n);
	memmove(exp + dst_off, exp + src_off, n);
	CHECK_PTR(ret, got + dst_off);
	CHECK(memcmp(got, exp, BUF) == 0,
		"dst=buf+%zu src=buf+%zu n=%zu:\n         expected %s\n"
		"         got      %s", dst_off, src_off, n,
		show(exp, BUF), show(got, BUF));
}

static void	no_overlap(void)
{
	compare(0, 16, 10);
	compare(16, 0, 10);
}

static void	overlap_dst_after_src(void)
{
	/* Must copy backwards or bytes get overwritten before being read. */
	compare(2, 0, 10);
	compare(1, 0, 31);
	compare(5, 3, 20);
}

static void	overlap_dst_before_src(void)
{
	compare(0, 2, 10);
	compare(0, 1, 31);
	compare(3, 5, 20);
}

static void	same_pointer(void)
{
	compare(4, 4, 10);
}

static void	n_zero(void)
{
	compare(0, 5, 0);
	compare(5, 0, 0);
}

static void	copies_past_nul(void)
{
	char	got[10] = "ab\0cd\0ef";
	char	exp[10] = "ab\0cd\0ef";

	ft_memmove(got + 1, got, 8);
	memmove(exp + 1, exp, 8);
	CHECK(memcmp(got, exp, 10) == 0, "expected %s, got %s",
		show(exp, 10), show(got, 10));
}

static void	separate_buffers(void)
{
	char	dst[16];
	char	*ret;

	memset(dst, 'X', sizeof(dst));
	ret = ft_memmove(dst, "hello", 6);
	CHECK_PTR(ret, dst);
	CHECK_STR(dst, "hello");
	CHECK_INT(dst[6], 'X');
}

static void	null_null_zero(void)
{
	CHECK_PTR(ft_memmove(opaque_ptr(NULL), opaque_ptr(NULL), 0), NULL);
}

static void	large_overlap(void)
{
	size_t	n = 1 << 20;
	char	*got = malloc(n + 7);
	char	*exp = malloc(n + 7);

	for (size_t i = 0; i < n + 7; i++)
		got[i] = exp[i] = (char)(i * 13);
	ft_memmove(got + 7, got, n);
	memmove(exp + 7, exp, n);
	CHECK_MEM(got, exp, n + 7);
	ft_memmove(got, got + 7, n);
	memmove(exp, exp + 7, n);
	CHECK_MEM(got, exp, n + 7);
	free(got);
	free(exp);
}

int	main(void)
{
	printf("ft_memmove\n");
	run_case("non-overlapping regions", no_overlap);
	run_case("overlap, dst > src (must copy backwards)", overlap_dst_after_src);
	run_case("overlap, dst < src (copy forwards)", overlap_dst_before_src);
	run_case("dst == src", same_pointer);
	run_case("n = 0 changes nothing", n_zero);
	run_case("copies past embedded '\\0'", copies_past_nul);
	run_case("separate buffers, returns dst, nothing past n", separate_buffers);
	run_case("dest = src = NULL with n = 0 -> NULL, no crash", null_null_zero);
	run_case("large overlapping move (1 MiB) both directions", large_overlap);
	return (finish("ft_memmove"));
}
