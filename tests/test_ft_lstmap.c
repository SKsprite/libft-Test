#include "lst_utils.h"

/*
** f allocates its result with raw_malloc (outside the allocation hook), so
** the failure-injection tests only ever fail ft_lstmap's own mallocs.
** g_f_live counts contents made by f and not yet released by del: if it is
** not 0 after a failed ft_lstmap, a content made by f was leaked.
*/
static long		g_f_live;
static size_t	g_f_calls;
static void		*g_f_args[1024];

static void	*dup_times2(void *content)
{
	int	*out = raw_malloc(sizeof(int));

	if (g_f_calls < 1024)
		g_f_args[g_f_calls] = content;
	g_f_calls++;
	*out = *(int *)content * 2;
	g_f_live++;
	return (out);
}

static void	del_int(void *content)
{
	g_ndel++;
	if (content)
	{
		g_f_live--;
		raw_free(content);
	}
}

static void	*strlen_of(void *content)
{
	return ((void *)(uintptr_t)strlen(content));
}

static void	*to_null(void *content)
{
	(void)content;
	return (NULL);
}

static void	reset(void)
{
	g_f_live = 0;
	g_f_calls = 0;
	g_ndel = 0;
}

/* Frees a list returned by ft_lstmap(…, dup_times2, …). */
static void	free_mapped(t_list *lst)
{
	size_t	i = 0;

	while (lst && i++ < LIST_CAP)
	{
		t_list	*next = lst->next;

		del_int(lst->content);
		free(lst);
		lst = next;
	}
}

static t_list	*int_list(int *v, size_t n)
{
	void	*c[64];

	for (size_t i = 0; i < n; i++)
		c[i] = &v[i];
	return (mk_list(c, n));
}

static void	basic(void)
{
	int		v[] = {1, 2, 3, 4, 5};
	t_list	*src = int_list(v, 5);
	t_list	*got;
	t_list	*p;
	int		i;

	reset();
	got = ft_lstmap(src, dup_times2, del_int);
	CHECK(got != NULL, "returned NULL");
	CHECK(list_len(got) == 5, "expected 5 nodes, got %zu", list_len(got));
	for (i = 0, p = got; p && i < 5; i++, p = p->next)
		CHECK(p->content && *(int *)p->content == v[i] * 2,
			"node %d: expected %d, got %d", i, v[i] * 2,
			p->content ? *(int *)p->content : -1);
	CHECK(g_ndel == 0, "del called %zu times on success (expected 0)",
		g_ndel);
	free_mapped(got);
	free_nodes(src);
}

static void	f_called_once_per_node_in_order(void)
{
	int		v[] = {1, 2, 3};
	t_list	*src = int_list(v, 3);
	t_list	*got;

	reset();
	got = ft_lstmap(src, dup_times2, del_int);
	CHECK(g_f_calls == 3, "3 nodes: f called %zu times", g_f_calls);
	for (size_t i = 0; i < 3 && i < g_f_calls; i++)
		CHECK(g_f_args[i] == &v[i], "call %zu: f got wrong content "
			"(node instead of content? wrong order?)", i);
	free_mapped(got);
	free_nodes(src);
}

static void	new_nodes_original_untouched(void)
{
	int		v[] = {1, 2, 3};
	t_list	*src = int_list(v, 3);
	t_list	*n0 = src;
	t_list	*n1 = src->next;
	t_list	*n2 = n1->next;
	t_list	*got;

	reset();
	got = ft_lstmap(src, dup_times2, del_int);
	for (t_list *p = got; p; p = p->next)
		CHECK(p != n0 && p != n1 && p != n2,
			"result reuses a node of the original list");
	CHECK(n0->next == n1 && n1->next == n2 && n2->next == NULL,
		"original list links were modified");
	CHECK(n0->content == &v[0] && n1->content == &v[1] && n2->content == &v[2],
		"original contents were modified");
	CHECK(v[0] == 1 && v[1] == 2 && v[2] == 3, "original ints were modified");
	free_mapped(got);
	free_nodes(src);
}

static void	last_next_null(void)
{
	int		v[] = {1, 2};
	t_list	*src = int_list(v, 2);
	t_list	*got;

	reset();
	got = ft_lstmap(src, dup_times2, del_int);
	CHECK(got && got->next && got->next->next == NULL,
		"last node's next must be NULL");
	free_mapped(got);
	free_nodes(src);
}

static void	one_node(void)
{
	int		v = 21;
	t_list	*src = mk_node(&v);
	t_list	*got;

	reset();
	got = ft_lstmap(src, dup_times2, del_int);
	CHECK(got && list_len(got) == 1, "expected 1 node");
	CHECK(got && got->content && *(int *)got->content == 42,
		"expected 42");
	free_mapped(got);
	free_nodes(src);
}

static void	strings_to_lengths(void)
{
	void	*c[] = {"", "a", "hello", "libft!"};
	t_list	*src = mk_list(c, 4);
	t_list	*got = ft_lstmap(src, strlen_of, NULL);
	size_t	exp[] = {0, 1, 5, 6};
	size_t	i = 0;

	CHECK(list_len(got) == 4, "expected 4 nodes, got %zu", list_len(got));
	for (t_list *p = got; p && i < 4; p = p->next, i++)
		CHECK((size_t)(uintptr_t)p->content == exp[i],
			"node %zu: expected %zu", i, exp[i]);
	free_nodes(got);
	free_nodes(src);
}

/* f returning NULL is a valid content; the new list keeps it. */
static void	f_returns_null(void)
{
	int		v[] = {1, 2};
	t_list	*src = int_list(v, 2);
	t_list	*got = ft_lstmap(src, to_null, NULL);

	CHECK(list_len(got) == 2, "f returning NULL content: expected 2 nodes, "
		"got %zu", list_len(got));
	free_nodes(got);
	free_nodes(src);
}

static void	empty_list(void)
{
	t_list	*got;

	reset();
	got = ft_lstmap(opaque_ptr(NULL), dup_times2, del_int);
	CHECK_PTR(got, NULL);
	CHECK(g_f_calls == 0, "empty list: f called %zu times", g_f_calls);
}

static void	large_list(void)
{
	static int	v = 3;
	t_list		*src = NULL;
	t_list		*got;

	for (int i = 0; i < 10000; i++)
	{
		t_list	*n = mk_node(&v);

		n->next = src;
		src = n;
	}
	reset();
	got = ft_lstmap(src, dup_times2, del_int);
	CHECK(list_len(got) == 10000, "expected 10000 nodes, got %zu",
		list_len(got));
	free_mapped(got);
	free_nodes(src);
}

/* Allocation failure: must return NULL and free ALL new nodes + contents. */
static t_list	*map_failing_at(t_list *src, long fail_at)
{
	t_list	*got;

	reset();
	hook_start(fail_at);
	got = ft_lstmap(src, dup_times2, del_int);
	hook_stop();
	return (got);
}

static void	expect_clean_fail(size_t n, long idx, long total)
{
	int		v[64];
	t_list	*src;
	t_list	*got;

	for (size_t i = 0; i < n; i++)
		v[i] = (int)i;
	src = int_list(v, n);
	got = map_failing_at(src, idx);
	CHECK(got == NULL, "%zu nodes, malloc %ld/%ld failed: must return NULL",
		n, idx + 1, total);
	CHECK(hook_nlive() == 0, "%zu nodes, malloc %ld/%ld failed: %zu node(s) "
		"leaked (new list not cleared?)", n, idx + 1, total, hook_nlive());
	CHECK(g_f_live == 0, "%zu nodes, malloc %ld/%ld failed: %ld content(s) "
		"made by f were not passed to del", n, idx + 1, total, g_f_live);
	CHECK_SIZE(list_len(src), n);
	if (got)
		free_mapped(got);
	hook_release();
	free_nodes(src);
}

static long	count_allocs(size_t n)
{
	int		v[64] = {0};
	t_list	*src = int_list(v, n);
	t_list	*got = map_failing_at(src, -1);
	long	total = hook_nalloc();

	free_mapped(got);
	hook_release();
	free_nodes(src);
	return (total);
}

static void	alloc_fail_first(void)
{
	expect_clean_fail(3, 0, count_allocs(3));
}

static void	alloc_fail_last(void)
{
	long	total = count_allocs(3);

	expect_clean_fail(3, total - 1, total);
}

static void	alloc_fail_every_point(void)
{
	size_t	sizes[] = {1, 2, 5, 20};

	for (size_t s = 0; s < 4; s++)
	{
		long	total = count_allocs(sizes[s]);

		for (long i = 0; i < total; i++)
			expect_clean_fail(sizes[s], i, total);
	}
}

static void	null_f(void)
{
	int		v = 1;
	t_list	*src = mk_node(&v);

	CHECK_PTR(ft_lstmap(src, opaque_ptr(NULL), del_int), NULL);
	free_nodes(src);
}

int	main(void)
{
	printf("ft_lstmap\n");
	run_case("5 ints -> doubled, same order", basic);
	run_case("f called once per node with its content, in order",
		f_called_once_per_node_in_order);
	run_case("new nodes; original list untouched",
		new_nodes_original_untouched);
	run_case("last node's next is NULL", last_next_null);
	run_case("1 node", one_node);
	run_case("strings -> lengths (del = NULL)", strings_to_lengths);
	run_case("f returns NULL content -> nodes still created", f_returns_null);
	run_case("empty list (NULL) -> NULL, f never called", empty_list);
	run_case("10000 nodes", large_list);
	run_case("alloc fail: 1st malloc -> NULL, no leak", alloc_fail_first);
	run_case("alloc fail: last malloc -> NULL, earlier nodes + contents "
		"freed", alloc_fail_last);
	run_case("alloc fail: every point (1, 2, 5, 20 nodes) -> NULL, no leak",
		alloc_fail_every_point);
	run_case("(defensive) f == NULL -> NULL", null_f);
	return (finish("ft_lstmap"));
}
