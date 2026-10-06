#include "lst_utils.h"

static int	g_v[] = {0, 1, 2, 3, 4};

static void	null_list(void)
{
	CHECK_PTR(ft_lstlast(opaque_ptr(NULL)), NULL);
}

static void	one_node(void)
{
	t_list	*node = mk_node(&g_v[0]);

	CHECK_PTR(ft_lstlast(node), node);
	free_nodes(node);
}

static void	several_nodes(void)
{
	void	*c[] = {&g_v[0], &g_v[1], &g_v[2], &g_v[3], &g_v[4]};
	t_list	*lst = mk_list(c, 5);
	t_list	*last = lst->next->next->next->next;
	t_list	*got = ft_lstlast(lst);

	CHECK(got == last, "returned %p, expected the 5th node %p",
		(void *)got, (void *)last);
	if (got)
		CHECK_PTR(got->content, &g_v[4]);
	free_nodes(lst);
}

static void	two_nodes(void)
{
	void	*c[] = {&g_v[0], &g_v[1]};
	t_list	*lst = mk_list(c, 2);

	CHECK_PTR(ft_lstlast(lst), lst->next);
	free_nodes(lst);
}

static void	last_with_null_content(void)
{
	void	*c[] = {&g_v[0], &g_v[1], NULL};
	t_list	*lst = mk_list(c, 3);

	CHECK(ft_lstlast(lst) == lst->next->next,
		"must return the last node even if its content is NULL");
	free_nodes(lst);
}

static void	returns_node_not_content(void)
{
	void	*c[] = {&g_v[0], &g_v[1]};
	t_list	*lst = mk_list(c, 2);

	CHECK(ft_lstlast(lst) != (t_list *)&g_v[1],
		"returned the content instead of the node");
	free_nodes(lst);
}

static void	list_unchanged(void)
{
	void	*c[] = {&g_v[0], &g_v[1], &g_v[2]};
	t_list	*lst = mk_list(c, 3);

	ft_lstlast(lst);
	CHECK_SIZE(list_len(lst), 3);
	CHECK_PTR(lst->content, &g_v[0]);
	free_nodes(lst);
}

static void	large_list(void)
{
	t_list	*head = NULL;
	t_list	*last = mk_node(NULL);

	head = last;
	for (int i = 0; i < 100000; i++)
	{
		t_list	*n = mk_node(NULL);

		n->next = head;
		head = n;
	}
	CHECK_PTR(ft_lstlast(head), last);
	free_nodes(head);
}

int	main(void)
{
	printf("ft_lstlast\n");
	run_case("NULL (empty list) -> NULL", null_list);
	run_case("1 node -> that node", one_node);
	run_case("2 nodes -> second", two_nodes);
	run_case("5 nodes -> 5th node", several_nodes);
	run_case("last node has NULL content", last_with_null_content);
	run_case("returns the node, not its content", returns_node_not_content);
	run_case("list is not modified", list_unchanged);
	run_case("100001 nodes", large_list);
	return (finish("ft_lstlast"));
}
