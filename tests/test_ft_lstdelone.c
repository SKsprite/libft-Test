#include "lst_utils.h"

static int	g_v[] = {0, 1, 2};

static void	del_gets_content(void)
{
	t_list	*node = mk_node(&g_v[0]);

	g_ndel = 0;
	hook_start(-1);
	ft_lstdelone(node, rec_del);
	hook_stop();
	CHECK(g_ndel == 1, "del must be called exactly once, was called %zu "
		"times", g_ndel);
	CHECK(g_ndel == 0 || g_del_log[0] == &g_v[0],
		"del must receive lst->content (%p), got %p%s", (void *)&g_v[0],
		g_del_log[0], g_del_log[0] == node ? " (the node itself)" : "");
	CHECK(hook_freed(node), "the node itself was not freed");
}

static void	node_freed(void)
{
	t_list	*node = mk_node(&g_v[0]);

	g_ndel = 0;
	hook_start(-1);
	ft_lstdelone(node, rec_del);
	hook_stop();
	CHECK(hook_freed(node), "the node itself was not freed");
	CHECK(!hook_freed(&g_v[0]), "free() was called on the content: only "
		"del may release the content");
}

static void	next_not_freed(void)
{
	void	*c[] = {&g_v[0], &g_v[1], &g_v[2]};
	t_list	*lst = mk_list(c, 3);
	t_list	*second = lst->next;

	g_ndel = 0;
	hook_start(-1);
	ft_lstdelone(lst, rec_del);
	hook_stop();
	CHECK(!hook_freed(second), "the next node was freed (must NOT be)");
	CHECK(!del_was_called_with(&g_v[1]), "del was called on the next "
		"node's content");
	CHECK(g_ndel == 1, "del called %zu times, expected 1", g_ndel);
	CHECK_PTR(second->content, &g_v[1]);
	CHECK_SIZE(list_len(second), 2);
	free_nodes(second);
}

static void	free_as_del(void)
{
	char	*s = strdup("heap content");
	t_list	*node = mk_node(s);

	hook_start(-1);
	ft_lstdelone(node, free);
	hook_stop();
	CHECK(hook_freed(s), "del (= free) was not called on the content");
	CHECK(hook_freed(node), "the node itself was not freed");
	CHECK(hook_nfree() == 2, "expected 2 frees (content + node), got %zu",
		hook_nfree());
}

static void	null_content(void)
{
	t_list	*node = mk_node(NULL);

	g_ndel = 0;
	hook_start(-1);
	ft_lstdelone(node, rec_del);
	hook_stop();
	CHECK(hook_freed(node), "node with NULL content was not freed");
}

static void	null_del(void)
{
	/*
	** Without del the content can't be released, so freeing the node would
	** lose the only pointer to it (leak) and the caller's later free(node)
	** becomes a double free. Expected: return without freeing anything.
	*/
	char	*s = strdup("heap content");
	t_list	*node = mk_node(s);
	int		node_freed;

	hook_start(-1);
	ft_lstdelone(node, NULL);
	hook_stop();
	node_freed = hook_freed(node);
	CHECK(!node_freed, "del == NULL: the node was freed (must return "
		"without freeing anything, the content would leak)");
	CHECK(!hook_freed(s), "del == NULL: the content was freed");
	CHECK(hook_nfree() == 0, "del == NULL: expected 0 frees, got %zu",
		hook_nfree());
	if (!node_freed)
	{
		CHECK_PTR(node->content, s);
		raw_free(node);
	}
	free(s);
}

static void	null_lst_null_del(void)
{
	hook_start(-1);
	ft_lstdelone(opaque_ptr(NULL), NULL);
	hook_stop();
	CHECK(hook_nfree() == 0, "free() called %zu times", hook_nfree());
}

static void	null_lst(void)
{
	g_ndel = 0;
	ft_lstdelone(opaque_ptr(NULL), rec_del);
	CHECK(g_ndel == 0, "lst == NULL: del must not be called (called %zu "
		"times)", g_ndel);
}

int	main(void)
{
	printf("ft_lstdelone\n");
	run_case("del called once with lst->content (not the node)",
		del_gets_content);
	run_case("node is freed, content only via del", node_freed);
	run_case("next node NOT freed, NOT del'd, still intact", next_not_freed);
	run_case("del = free -> content and node freed", free_as_del);
	run_case("node with NULL content still freed", null_content);
	run_case("(defensive) lst == NULL -> no crash, del not called",
		null_lst);
	run_case("(defensive) del == NULL -> node and content NOT freed",
		null_del);
	run_case("(defensive) lst == NULL, del == NULL -> no crash",
		null_lst_null_del);
	return (finish("ft_lstdelone"));
}
