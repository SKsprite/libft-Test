#include "lst_utils.h"

static int	g_v[] = {0, 1, 2, 3, 4};

static void	add_to_empty_list(void)
{
	t_list	*head = NULL;
	t_list	*node = mk_node(&g_v[1]);

	ft_lstadd_front(&head, node);
	CHECK_PTR(head, node);
	CHECK_PTR(node->next, NULL);
	free_nodes(head);
}

static void	add_to_one_node(void)
{
	t_list	*old = mk_node(&g_v[1]);
	t_list	*head = old;
	t_list	*node = mk_node(&g_v[0]);

	ft_lstadd_front(&head, node);
	CHECK(head == node, "head does not point at the new node");
	CHECK(node->next == old, "new->next does not point at the old head");
	CHECK_PTR(old->next, NULL);
	free_nodes(head);
}

static void	add_to_longer_list(void)
{
	void	*c[] = {&g_v[1], &g_v[2], &g_v[3]};
	t_list	*head = mk_list(c, 3);
	t_list	*old = head;
	t_list	*node = mk_node(&g_v[0]);

	ft_lstadd_front(&head, node);
	CHECK_PTR(head, node);
	CHECK_PTR(head->next, old);
	CHECK_SIZE(list_len(head), 4);
	for (int i = 0; head && i < 4; i++)
	{
		t_list	*n = head;

		for (int j = 0; j < i && n; j++)
			n = n->next;
		CHECK(n && n->content == &g_v[i], "node %d has wrong content", i);
	}
	free_nodes(head);
}

static void	repeated_adds_reverse_order(void)
{
	t_list	*head = NULL;
	t_list	*n;
	int		i;

	for (i = 0; i < 5; i++)
		ft_lstadd_front(&head, mk_node(&g_v[i]));
	CHECK_SIZE(list_len(head), 5);
	n = head;
	for (i = 4; i >= 0 && n; i--, n = n->next)
		CHECK(n->content == &g_v[i], "expected content %d at position %d",
			i, 4 - i);
	free_nodes(head);
}

static void	content_untouched(void)
{
	t_list	*head = NULL;
	t_list	*node = mk_node(&g_v[3]);

	ft_lstadd_front(&head, node);
	CHECK_PTR(node->content, &g_v[3]);
	CHECK_INT(g_v[3], 3);
	free_nodes(head);
}

static void	no_allocation(void)
{
	t_list	*head = NULL;
	t_list	*node = mk_node(&g_v[0]);

	hook_start(-1);
	ft_lstadd_front(&head, node);
	hook_stop();
	CHECK(hook_nalloc() == 0, "ft_lstadd_front should not allocate "
		"(made %ld allocations)", hook_nalloc());
	free_nodes(head);
}

/* Not required by the subject, but evaluators/testers often try these. */
static void	null_new(void)
{
	t_list	*old = mk_node(&g_v[1]);
	t_list	*head = old;

	ft_lstadd_front(&head, opaque_ptr(NULL));
	CHECK(head == old, "new = NULL must leave the list unchanged");
	free_nodes(old);
}

static void	null_lst(void)
{
	t_list	*node = mk_node(&g_v[0]);

	ft_lstadd_front(opaque_ptr(NULL), node);
	free_nodes(node);
}

int	main(void)
{
	printf("ft_lstadd_front\n");
	run_case("empty list (*lst == NULL) -> new is head, new->next NULL",
		add_to_empty_list);
	run_case("one node -> new becomes head, points at old head",
		add_to_one_node);
	run_case("3 nodes -> 4 nodes, order kept", add_to_longer_list);
	run_case("5 adds to empty list -> reverse order", repeated_adds_reverse_order);
	run_case("content not modified", content_untouched);
	run_case("does not allocate", no_allocation);
	run_case("(defensive) new == NULL -> list unchanged, no crash", null_new);
	run_case("(defensive) lst == NULL -> no crash", null_lst);
	return (finish("ft_lstadd_front"));
}
