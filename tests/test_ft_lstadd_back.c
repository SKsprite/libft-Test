#include "lst_utils.h"

static int	g_v[] = {0, 1, 2, 3, 4};

static void	add_to_empty_list(void)
{
	t_list	*head = NULL;
	t_list	*node = mk_node(&g_v[0]);

	ft_lstadd_back(&head, node);
	CHECK(head == node, "*lst was NULL: it must now point at new");
	CHECK_PTR(node->next, NULL);
	free_nodes(head);
}

static void	add_to_one_node(void)
{
	t_list	*head = mk_node(&g_v[0]);
	t_list	*first = head;
	t_list	*node = mk_node(&g_v[1]);

	ft_lstadd_back(&head, node);
	CHECK(head == first, "head must not change");
	CHECK(first->next == node, "old last node does not point at new");
	CHECK_PTR(node->next, NULL);
	free_nodes(head);
}

static void	add_to_longer_list(void)
{
	void	*c[] = {&g_v[0], &g_v[1], &g_v[2]};
	t_list	*head = mk_list(c, 3);
	t_list	*first = head;
	t_list	*node = mk_node(&g_v[3]);

	ft_lstadd_back(&head, node);
	CHECK_PTR(head, first);
	CHECK_SIZE(list_len(head), 4);
	CHECK_PTR(head->next->next->next, node);
	CHECK_PTR(head->next->next->content, &g_v[2]);
	free_nodes(head);
}

static void	repeated_adds_keep_order(void)
{
	t_list	*head = NULL;
	t_list	*n;
	int		i;

	for (i = 0; i < 5; i++)
		ft_lstadd_back(&head, mk_node(&g_v[i]));
	CHECK_SIZE(list_len(head), 5);
	n = head;
	for (i = 0; i < 5 && n; i++, n = n->next)
		CHECK(n->content == &g_v[i], "position %d: wrong content", i);
	free_nodes(head);
}

static void	append_a_chain(void)
{
	void	*a[] = {&g_v[0], &g_v[1]};
	void	*b[] = {&g_v[2], &g_v[3], &g_v[4]};
	t_list	*head = mk_list(a, 2);
	t_list	*tail = mk_list(b, 3);

	ft_lstadd_back(&head, tail);
	CHECK(list_len(head) == 5, "appending a 3-node chain to 2 nodes: "
		"expected 5 nodes, got %zu (new->next was overwritten?)",
		list_len(head));
	free_nodes(head);
}

static void	no_allocation(void)
{
	t_list	*head = mk_node(&g_v[0]);
	t_list	*node = mk_node(&g_v[1]);

	hook_start(-1);
	ft_lstadd_back(&head, node);
	hook_stop();
	CHECK(hook_nalloc() == 0, "ft_lstadd_back should not allocate "
		"(made %ld allocations)", hook_nalloc());
	free_nodes(head);
}

static void	null_new(void)
{
	void	*c[] = {&g_v[0], &g_v[1]};
	t_list	*head = mk_list(c, 2);
	t_list	*first = head;

	ft_lstadd_back(&head, opaque_ptr(NULL));
	CHECK_PTR(head, first);
	CHECK_SIZE(list_len(head), 2);
	free_nodes(head);
}

static void	null_new_empty_list(void)
{
	t_list	*head = NULL;

	ft_lstadd_back(&head, opaque_ptr(NULL));
	CHECK_PTR(head, NULL);
}

static void	null_lst(void)
{
	t_list	*node = mk_node(&g_v[0]);

	ft_lstadd_back(opaque_ptr(NULL), node);
	free_nodes(node);
}

int	main(void)
{
	printf("ft_lstadd_back\n");
	run_case("empty list (*lst == NULL) -> *lst = new", add_to_empty_list);
	run_case("one node -> new is second, head unchanged", add_to_one_node);
	run_case("3 nodes -> new is 4th", add_to_longer_list);
	run_case("5 adds to empty list -> same order", repeated_adds_keep_order);
	run_case("new is a chain -> whole chain appended", append_a_chain);
	run_case("does not allocate", no_allocation);
	run_case("(defensive) new == NULL -> list unchanged", null_new);
	run_case("(defensive) new == NULL on empty list -> still NULL", null_new_empty_list);
	run_case("(defensive) lst == NULL -> no crash", null_lst);
	return (finish("ft_lstadd_back"));
}
