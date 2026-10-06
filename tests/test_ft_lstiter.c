#include "lst_utils.h"

static void		*g_seen[1024];
static size_t	g_calls;

static void	record(void *content)
{
	if (g_calls < 1024)
		g_seen[g_calls] = content;
	g_calls++;
}

static void	increment(void *content)
{
	(*(int *)content)++;
}

static void	upcase(void *content)
{
	for (char *s = content; *s; s++)
		*s = (char)toupper((unsigned char)*s);
}

static void	called_on_each_in_order(void)
{
	int		v[] = {10, 20, 30, 40};
	void	*c[] = {&v[0], &v[1], &v[2], &v[3]};
	t_list	*lst = mk_list(c, 4);

	g_calls = 0;
	ft_lstiter(lst, record);
	CHECK(g_calls == 4, "4 nodes: f called %zu times", g_calls);
	for (size_t i = 0; i < 4 && i < g_calls; i++)
		CHECK(g_seen[i] == c[i], "call %zu: f did not get node %zu's "
			"content", i, i);
	free_nodes(lst);
}

static void	f_gets_content_not_node(void)
{
	int		v = 7;
	t_list	*lst = mk_node(&v);

	g_calls = 0;
	ft_lstiter(lst, record);
	CHECK(g_calls == 0 || g_seen[0] != lst,
		"f received the node instead of its content");
	free_nodes(lst);
}

static void	modifies_in_place(void)
{
	int		v[] = {1, 2, 3};
	void	*c[] = {&v[0], &v[1], &v[2]};
	t_list	*lst = mk_list(c, 3);

	ft_lstiter(lst, increment);
	CHECK(v[0] == 2 && v[1] == 3 && v[2] == 4,
		"after increment: expected {2,3,4}, got {%d,%d,%d}", v[0], v[1], v[2]);
	free_nodes(lst);
}

static void	strings(void)
{
	char	a[] = "hello";
	char	b[] = "world";
	void	*c[] = {a, b};
	t_list	*lst = mk_list(c, 2);

	ft_lstiter(lst, upcase);
	CHECK_STR(a, "HELLO");
	CHECK_STR(b, "WORLD");
	free_nodes(lst);
}

static void	structure_unchanged(void)
{
	int		v[] = {1, 2, 3};
	void	*c[] = {&v[0], &v[1], &v[2]};
	t_list	*lst = mk_list(c, 3);
	t_list	*second = lst->next;
	t_list	*third = second->next;

	ft_lstiter(lst, increment);
	CHECK_PTR(lst->next, second);
	CHECK_PTR(second->next, third);
	CHECK_PTR(third->next, NULL);
	CHECK_PTR(lst->content, &v[0]);
	free_nodes(lst);
}

static void	one_node(void)
{
	int		v = 5;
	t_list	*lst = mk_node(&v);

	ft_lstiter(lst, increment);
	CHECK_INT(v, 6);
	free_nodes(lst);
}

static void	null_content(void)
{
	void	*c[] = {NULL, NULL};
	t_list	*lst = mk_list(c, 2);

	g_calls = 0;
	ft_lstiter(lst, record);
	CHECK(g_calls == 2, "nodes with NULL content: f called %zu times, "
		"expected 2", g_calls);
	free_nodes(lst);
}

static void	empty_list(void)
{
	g_calls = 0;
	ft_lstiter(opaque_ptr(NULL), record);
	CHECK(g_calls == 0, "empty list: f called %zu times", g_calls);
}

static void	no_allocation(void)
{
	int		v[] = {1, 2};
	void	*c[] = {&v[0], &v[1]};
	t_list	*lst = mk_list(c, 2);

	hook_start(-1);
	ft_lstiter(lst, increment);
	hook_stop();
	CHECK(hook_nalloc() == 0 && hook_nfree() == 0,
		"ft_lstiter should not malloc or free");
	free_nodes(lst);
}

static void	null_f(void)
{
	int		v = 1;
	t_list	*lst = mk_node(&v);

	ft_lstiter(lst, opaque_ptr(NULL));
	free_nodes(lst);
}

int	main(void)
{
	printf("ft_lstiter\n");
	run_case("f called once per node, in order", called_on_each_in_order);
	run_case("f receives content, not the node", f_gets_content_not_node);
	run_case("f modifies ints in place", modifies_in_place);
	run_case("f modifies strings in place", strings);
	run_case("list links are not modified", structure_unchanged);
	run_case("1 node", one_node);
	run_case("nodes with NULL content still visited", null_content);
	run_case("empty list (NULL) -> f never called", empty_list);
	run_case("does not malloc/free", no_allocation);
	run_case("(defensive) f == NULL -> no crash", null_f);
	return (finish("ft_lstiter"));
}
