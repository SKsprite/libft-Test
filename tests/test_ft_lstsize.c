#include "lst_utils.h"

static t_list	*list_of(size_t n)
{
	t_list	*head = NULL;

	while (n--)
	{
		t_list	*node = mk_node(NULL);

		node->next = head;
		head = node;
	}
	return (head);
}

static void	expect_size(size_t n)
{
	t_list	*lst = list_of(n);

	CHECK(ft_lstsize(lst) == n, "list of %zu nodes: expected %zu, got %u",
		n, n, ft_lstsize(lst));
	free_nodes(lst);
}

static void	null_list(void)
{
	CHECK(ft_lstsize(opaque_ptr(NULL)) == 0,
		"NULL (empty list): expected 0, got %u", ft_lstsize(NULL));
}

static void	one_node(void)
{
	expect_size(1);
}

static void	two_nodes(void)
{
	expect_size(2);
}

static void	five_nodes(void)
{
	expect_size(5);
}

static void	sizes_0_to_50(void)
{
	for (size_t n = 0; n <= 50; n++)
		expect_size(n);
}

static void	large_list(void)
{
	expect_size(100000);
}

static void	null_content_counted(void)
{
	void	*c[] = {NULL, NULL, NULL};
	t_list	*lst = mk_list(c, 3);

	CHECK(ft_lstsize(lst) == 3, "nodes with NULL content must be counted: "
		"expected 3, got %u", ft_lstsize(lst));
	free_nodes(lst);
}

static void	list_unchanged(void)
{
	int		v[] = {1, 2, 3};
	void	*c[] = {&v[0], &v[1], &v[2]};
	t_list	*lst = mk_list(c, 3);
	t_list	*second = lst->next;

	ft_lstsize(lst);
	CHECK_PTR(lst->next, second);
	CHECK_PTR(lst->content, &v[0]);
	CHECK_SIZE(list_len(lst), 3);
	free_nodes(lst);
}

static void	middle_of_list(void)
{
	t_list	*lst = list_of(5);

	CHECK(ft_lstsize(lst->next->next) == 3,
		"starting at the 3rd of 5 nodes: expected 3, got %u",
		ft_lstsize(lst->next->next));
	free_nodes(lst);
}

int	main(void)
{
	printf("ft_lstsize\n");
	run_case("NULL (empty list) -> 0", null_list);
	run_case("1 node -> 1", one_node);
	run_case("2 nodes -> 2", two_nodes);
	run_case("5 nodes -> 5", five_nodes);
	run_case("every size from 0 to 50", sizes_0_to_50);
	run_case("100000 nodes", large_list);
	run_case("nodes with NULL content are counted", null_content_counted);
	run_case("list is not modified", list_unchanged);
	run_case("start from the middle of a list", middle_of_list);
	return (finish("ft_lstsize"));
}
