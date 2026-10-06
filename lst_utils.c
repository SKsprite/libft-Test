#include "lst_utils.h"

void	*__real_malloc(size_t size);
void	*__real_calloc(size_t n, size_t size);
void	__real_free(void *p);
void	*__wrap_malloc(size_t size);
void	*__wrap_calloc(size_t n, size_t size);
void	__wrap_free(void *p);

#define MAX_TRACKED 4096

static int		g_armed = 0;
static long		g_fail_at = -1;
static long		g_nalloc = 0;
static void		*g_live[MAX_TRACKED];
static size_t	g_nlive = 0;
static void		*g_freed[MAX_TRACKED];
static size_t	g_nfreed = 0;

void			*g_del_log[DEL_LOG_MAX];
size_t			g_ndel = 0;

static void	*track(void *p)
{
	if (p && g_nlive < MAX_TRACKED)
		g_live[g_nlive++] = p;
	return (p);
}

void	*__wrap_malloc(size_t size)
{
	if (!g_armed)
		return (__real_malloc(size));
	if (g_nalloc++ == g_fail_at)
		return (NULL);
	return (track(__real_malloc(size)));
}

void	*__wrap_calloc(size_t n, size_t size)
{
	if (!g_armed)
		return (__real_calloc(n, size));
	if (g_nalloc++ == g_fail_at)
		return (NULL);
	return (track(__real_calloc(n, size)));
}

void	__wrap_free(void *p)
{
	if (g_armed && p && g_nfreed < MAX_TRACKED)
		g_freed[g_nfreed++] = p;
	for (size_t i = 0; p && i < g_nlive; i++)
	{
		if (g_live[i] == p)
		{
			g_live[i] = g_live[--g_nlive];
			break ;
		}
	}
	__real_free(p);
}

void	hook_start(long fail_at)
{
	g_nlive = 0;
	g_nfreed = 0;
	g_nalloc = 0;
	g_fail_at = fail_at;
	g_armed = 1;
}

void	hook_stop(void)
{
	g_armed = 0;
}

long	hook_nalloc(void)
{
	return (g_nalloc);
}

size_t	hook_nlive(void)
{
	return (g_nlive);
}

size_t	hook_nfree(void)
{
	return (g_nfreed);
}

int	hook_freed(const void *p)
{
	for (size_t i = 0; i < g_nfreed; i++)
		if (g_freed[i] == p)
			return (1);
	return (0);
}

/* Frees every block allocated (and not freed) during the last hook window. */
void	hook_release(void)
{
	while (g_nlive)
		free(g_live[g_nlive - 1]);
}

void	*raw_malloc(size_t size)
{
	return (__real_malloc(size));
}

void	raw_free(void *p)
{
	__real_free(p);
}

t_list	*mk_node(void *content)
{
	t_list	*node = raw_malloc(sizeof(t_list));

	if (!node)
	{
		perror("malloc");
		exit(2);
	}
	node->content = content;
	node->next = NULL;
	return (node);
}

t_list	*mk_list(void **contents, size_t n)
{
	t_list	*head = NULL;

	while (n--)
	{
		t_list	*node = mk_node(contents[n]);

		node->next = head;
		head = node;
	}
	return (head);
}

void	free_nodes(t_list *lst)
{
	size_t	i = 0;

	while (lst && i++ < LIST_CAP)
	{
		t_list	*next = lst->next;

		free(lst);
		lst = next;
	}
}

size_t	list_len(t_list *lst)
{
	size_t	n = 0;

	while (lst && n < LIST_CAP)
	{
		n++;
		lst = lst->next;
	}
	return (n);
}

void	rec_del(void *content)
{
	if (g_ndel < DEL_LOG_MAX)
		g_del_log[g_ndel] = content;
	g_ndel++;
}

int	del_was_called_with(const void *p)
{
	for (size_t i = 0; i < g_ndel && i < DEL_LOG_MAX; i++)
		if (g_del_log[i] == p)
			return (1);
	return (0);
}
