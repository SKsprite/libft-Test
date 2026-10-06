#include "lst_utils.h"

static int	g_v[] = {0, 1, 2, 3, 4};

static void	clear_list(size_t n)
{
	void	*c[5];
	t_list	*nodes[5];
	t_list	*head;
	t_list	*p;
	size_t	i;

	for (i = 0; i < n; i++)
		c[i] = &g_v[i];
	head = mk_list(c, n);
	for (i = 0, p = head; i < n; i++, p = p->next)
		nodes[i] = p;
	g_ndel = 0;
	hook_start(-1);
	ft_lstclear(&head, rec_del);
	hook_stop();
	CHECK(head == NULL, "%zu nodes: *lst must be NULL afterwards", n);
	CHECK(g_ndel == n, "%zu nodes: del called %zu times, expected %zu",
		n, g_ndel, n);
	for (i = 0; i < n; i++)
	{
		CHECK(del_was_called_with(&g_v[i]), "%zu nodes: del not called on "
			"content of node %zu", n, i);
		CHECK(hook_freed(nodes[i]), "%zu nodes: node %zu was not freed",
			n, i);
	}
	CHECK(!del_was_called_with(nodes[0]), "del received a node instead of "
		"its content");
}

static void	one_node(void)
{
	clear_list(1);
}

static void	five_nodes(void)
{
	clear_list(5);
}

static void	del_in_order(void)
{
	void	*c[] = {&g_v[0], &g_v[1], &g_v[2], &g_v[3], &g_v[4]};
	t_list	*head = mk_list(c, 5);

	g_ndel = 0;
	ft_lstclear(&head, rec_del);
	for (size_t i = 0; i < 5 && i < g_ndel; i++)
		CHECK(g_del_log[i] == &g_v[i], "call %zu: del got wrong content", i);
}

static void	free_as_del(void)
{
	void	*c[3];
	t_list	*head;

	for (int i = 0; i < 3; i++)
		c[i] = strdup("heap");
	head = mk_list(c, 3);
	hook_start(-1);
	ft_lstclear(&head, free);
	hook_stop();
	CHECK_PTR(head, NULL);
	CHECK(hook_nfree() == 6, "3 nodes with heap content: expected 6 frees "
		"(3 contents + 3 nodes), got %zu", hook_nfree());
}

static void	empty_list(void)
{
	t_list	*head = NULL;

	g_ndel = 0;
	ft_lstclear(&head, rec_del);
	CHECK_PTR(head, NULL);
	CHECK(g_ndel == 0, "empty list: del called %zu times", g_ndel);
}

/* Clearing from the middle: only that node and its successors go. */
static void	clear_from_middle(void)
{
	void	*c[] = {&g_v[0], &g_v[1], &g_v[2], &g_v[3]};
	t_list	*head = mk_list(c, 4);
	t_list	*first = head;

	g_ndel = 0;
	hook_start(-1);
	ft_lstclear(&head->next->next, rec_del);
	hook_stop();
	CHECK(first->next->next == NULL, "pointer to the cleared part (the "
		"2nd node's next) must be set to NULL");
	CHECK(g_ndel == 2, "del called %zu times, expected 2", g_ndel);
	CHECK(!hook_freed(first) && !hook_freed(first->next),
		"nodes before the cleared one were freed");
	CHECK_SIZE(list_len(head), 2);
	free_nodes(head);
}

static void	large_list(void)
{
	t_list	*head = NULL;

	for (int i = 0; i < 100000; i++)
	{
		t_list	*n = mk_node(NULL);

		n->next = head;
		head = n;
	}
	g_ndel = 0;
	ft_lstclear(&head, rec_del);
	CHECK_PTR(head, NULL);
	CHECK(g_ndel == 100000, "del called %zu times, expected 100000", g_ndel);
}

static void	null_lst(void)
{
	g_ndel = 0;
	ft_lstclear(opaque_ptr(NULL), rec_del);
	CHECK(g_ndel == 0, "lst == NULL: del called %zu times", g_ndel);
}

int	main(void)
{
	printf("ft_lstclear\n");
	run_case("1 node -> del'd, freed, *lst = NULL", one_node);
	run_case("5 nodes -> all del'd and freed, *lst = NULL", five_nodes);
	run_case("del called on each content in order", del_in_order);
	run_case("del = free -> every content and node freed", free_as_del);
	run_case("empty list (*lst == NULL) -> no crash, del not called",
		empty_list);
	run_case("clear from the middle -> earlier nodes kept", clear_from_middle);
	run_case("100000 nodes", large_list);
	run_case("(defensive) lst == NULL -> no crash", null_lst);
	return (finish("ft_lstclear"));
}
