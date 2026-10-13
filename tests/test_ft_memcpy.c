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

/*
** NULL guards. libc memcpy does NOT check for NULL: it returns dest
** untouched when n == 0 and crashes when it has to touch a NULL pointer.
** A guard like `if (!dst || !src) return (NULL);` breaks the n == 0 cases
** below and hides the crash. The usual 42 guard `if (!dst && !src)` only
** changes the NULL, NULL case, which is allowed (libc returns NULL there
** too when n == 0, and testers such as Tripouille expect no crash for n > 0).
*/
static char			g_buf[8];
static void			*g_dst;
static const void	*g_src;
static size_t		g_n;

static void	call_ft(void)
{
	ft_memcpy(g_dst, g_src, g_n);
}

static void	call_libc(void)
{
	memcpy(g_dst, g_src, g_n);
}

static void	same_crash_as_libc(void *dst, const void *src, size_t n,
	const char *args)
{
	int	ft;
	int	lc;

	if (under_valgrind())
		skip_case("crash test: skipped under valgrind (it reports the "
			"intended segfault as an error). Run `make memcpy` instead.");
	g_dst = dst;
	g_src = src;
	g_n = n;
	ft = crashes(call_ft);
	lc = crashes(call_libc);
	CHECK(ft == lc, "ft_memcpy(%s): libc memcpy %s, yours %s", args,
		lc ? "crashes" : "does not crash",
		ft ? "crashes" : "returned without crashing (NULL guard?)");
}

static void	src_null_n_zero(void)
{
	void	*ret;

	memset(g_buf, 'X', sizeof(g_buf));
	ret = ft_memcpy(g_buf, opaque_ptr(NULL), 0);
	CHECK(ret == g_buf, "ft_memcpy(buf, NULL, 0) must return buf like libc "
		"(got %p, expected %p): a `!src` guard returning NULL is wrong",
		ret, (void *)g_buf);
	CHECK_MEM(g_buf, "XXXXXXXX", 8);
}

static void	dst_null_n_zero(void)
{
	CHECK_PTR(ft_memcpy(opaque_ptr(NULL), "abc", 0), NULL);
}

static void	null_null_n(void)
{
	/* Tripouille: ft_memcpy(NULL, NULL, 3) == NULL, no crash. */
	CHECK_PTR(ft_memcpy(opaque_ptr(NULL), opaque_ptr(NULL), 3), NULL);
}

static void	dst_null_crashes(void)
{
	same_crash_as_libc(opaque_ptr(NULL), "abc", 3, "NULL, \"abc\", 3");
}

static void	src_null_crashes(void)
{
	same_crash_as_libc(g_buf, opaque_ptr(NULL), 3, "buf, NULL, 3");
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
	run_case("dst = buf, src = NULL, n = 0 -> returns buf, no write",
		src_null_n_zero);
	run_case("dst = NULL, src = \"abc\", n = 0 -> NULL, no crash",
		dst_null_n_zero);
	run_case("dst = src = NULL, n = 3 -> NULL, no crash", null_null_n);
	run_case("(strict) dst = NULL, n = 3 -> crashes like libc",
		dst_null_crashes);
	run_case("(strict) src = NULL, n = 3 -> crashes like libc",
		src_null_crashes);
	run_case("struct copy", struct_copy);
	run_case("large buffer (1 MiB)", large_buffer);
	return (finish("ft_memcpy"));
}
