#include "lst_utils.h"

static void	stores_content(void)
{
	int		x = 42;
	t_list	*node = ft_lstnew(&x);

	CHECK(node != NULL, "returned NULL");
	if (!node)
		return ;
	CHECK_PTR(node->content, &x);
	CHECK(*(int *)node->content == 42, "content does not point at 42");
	free(node);
}

static void	next_is_null(void)
{
	int		x = 1;
	t_list	*node = ft_lstnew(&x);

	CHECK(node != NULL, "returned NULL");
	if (!node)
		return ;
	CHECK_PTR(node->next, NULL);
	free(node);
}

static void	string_content_not_copied(void)
{
	char	s[] = "hello";
	t_list	*node = ft_lstnew(s);

	CHECK(node != NULL, "returned NULL");
	if (!node)
		return ;
	CHECK(node->content == s,
		"content must be the same pointer, not a copy of the string");
	free(node);
}

static void	null_content(void)
{
	t_list	*node = ft_lstnew(NULL);

	CHECK(node != NULL, "NULL content is valid: must still return a node");
	if (!node)
		return ;
	CHECK_PTR(node->content, NULL);
	CHECK_PTR(node->next, NULL);
	free(node);
}

static void	new_node_each_call(void)
{
	int		x = 0;
	t_list	*a = ft_lstnew(&x);
	t_list	*b = ft_lstnew(&x);

	CHECK(a && b, "returned NULL");
	CHECK(a != b, "two calls returned the same node");
	free(a);
	free(b);
}

static void	one_allocation(void)
{
	int		x = 0;
	t_list	*node;

	hook_start(-1);
	node = ft_lstnew(&x);
	hook_stop();
	CHECK(hook_nalloc() == 1, "expected exactly 1 malloc, got %ld",
		hook_nalloc());
	free(node);
}

static void	alloc_fail(void)
{
	int		x = 0;
	t_list	*node;

	hook_start(0);
	node = ft_lstnew(&x);
	hook_stop();
	CHECK_PTR(node, NULL);
	CHECK(hook_nlive() == 0, "%zu block(s) leaked", hook_nlive());
	hook_release();
}

int	main(void)
{
	printf("ft_lstnew\n");
	run_case("content is stored", stores_content);
	run_case("next is NULL", next_is_null);
	run_case("content is the same pointer (not copied)",
		string_content_not_copied);
	run_case("NULL content -> valid node with content NULL", null_content);
	run_case("each call returns a new node", new_node_each_call);
	run_case("makes exactly one allocation", one_allocation);
	run_case("malloc fails -> NULL", alloc_fail);
	return (finish("ft_lstnew"));
}
