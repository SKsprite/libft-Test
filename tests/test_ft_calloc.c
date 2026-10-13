#include "test.h"

static int	all_zero(const void *p, size_t n)
{
	const unsigned char	*b = p;

	for (size_t i = 0; i < n; i++)
		if (b[i])
			return (0);
	return (1);
}

static void	zeroed_memory(void)
{
	char	*dirty;
	char	*p;

	/*
	** Dirty a chunk first so malloc likely hands it back to ft_calloc:
	** a calloc that forgets to zero will then return garbage.
	*/
	dirty = malloc(64);
	memset(dirty, 0xAA, 64);
	free(dirty);
	p = ft_calloc(8, 8);
	CHECK(p != NULL, "ft_calloc(8, 8) returned NULL");
	if (p)
		CHECK(all_zero(p, 64), "memory not zeroed: %s", show(p, 64));
	free(p);
}

static void	int_array(void)
{
	int	*p = ft_calloc(10, sizeof(int));

	CHECK(p != NULL, "ft_calloc(10, sizeof(int)) returned NULL");
	if (!p)
		return ;
	for (int i = 0; i < 10; i++)
		CHECK_INT(p[i], 0);
	for (int i = 0; i < 10; i++)
		p[i] = i;
	free(p);
}

static void	zero_sizes_are_freeable(void)
{
	/* Either NULL or a unique pointer is fine, but it must be free()-able. */
	free(ft_calloc(0, 0));
	free(ft_calloc(0, 10));
	free(ft_calloc(10, 0));
}

static void	zero_sizes_like_glibc(void)
{
	/* glibc returns a unique non-NULL pointer here; Sfabi's tester requires it. */
	size_t	args[][2] = {{0, 10}, {10, 0}, {1, 0}, {0, 0}};
	void	*p;

	for (size_t i = 0; i < 4; i++)
	{
		p = ft_calloc(args[i][0], args[i][1]);
		CHECK(p != NULL, "ft_calloc(%zu, %zu) returned NULL (glibc returns a "
			"free()-able non-NULL pointer)", args[i][0], args[i][1]);
		free(p);
	}
}

static void	overflow_returns_null(void)
{
	volatile size_t	big = SIZE_MAX;
	volatile size_t	half = SIZE_MAX / 2 + 2;
	void			*p;

	p = ft_calloc(big, 2);
	CHECK(p == NULL, "ft_calloc(SIZE_MAX, 2) should be NULL (overflow), got %p", p);
	free(p);
	p = ft_calloc(2, big);
	CHECK(p == NULL, "ft_calloc(2, SIZE_MAX) should be NULL (overflow), got %p", p);
	free(p);
	p = ft_calloc(big, big);
	CHECK(p == NULL, "ft_calloc(SIZE_MAX, SIZE_MAX) should be NULL (overflow), got %p", p);
	free(p);
	p = ft_calloc(half, 2);
	CHECK(p == NULL, "ft_calloc(SIZE_MAX/2+2, 2) should be NULL (overflow), got %p", p);
	free(p);
}

static void	large_allocation(void)
{
	size_t	n = 1 << 20;
	char	*p = ft_calloc(n, 1);

	CHECK(p != NULL, "ft_calloc(1MiB, 1) returned NULL");
	if (p)
		CHECK(all_zero(p, n), "1 MiB block not fully zeroed");
	free(p);
}

static void	many_small(void)
{
	for (int i = 1; i < 200; i++)
	{
		char	*p = ft_calloc((size_t)i, 3);

		CHECK(p != NULL && all_zero(p, (size_t)i * 3),
			"ft_calloc(%d, 3) not zeroed or NULL", i);
		if (p)
			memset(p, 0x55, (size_t)i * 3);
		free(p);
	}
}

int	main(void)
{
	printf("ft_calloc\n");
	run_case("memory is zeroed (even if reused)", zeroed_memory);
	run_case("int array is zeroed and writable", int_array);
	run_case("nmemb or size = 0 -> free()-able result", zero_sizes_are_freeable);
	run_case("(glibc) nmemb or size = 0 -> non-NULL", zero_sizes_like_glibc);
	run_case("nmemb * size overflow -> NULL", overflow_returns_null);
	run_case("large allocation (1 MiB) zeroed", large_allocation);
	run_case("many small allocations, reuse, all zeroed", many_small);
	return (finish("ft_calloc"));
}
