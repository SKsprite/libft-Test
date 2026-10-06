#ifndef LST_UTILS_H
# define LST_UTILS_H

# include "test.h"

/*
** Helpers for the Part 3 (linked list) tests.
**
** Allocation hook: the lst tests are linked with
** -Wl,--wrap=malloc,--wrap=calloc,--wrap=free (see Makefile), so every
** malloc/free libft makes goes through lst_utils.c. Between hook_start()
** and hook_stop():
**   - allocation number `fail_at` (0-based) returns NULL (-1 = never),
**   - every block allocated is tracked until freed (hook_nlive),
**   - every pointer passed to free() is logged (hook_freed).
*/
void	hook_start(long fail_at);
void	hook_stop(void);
long	hook_nalloc(void);
size_t	hook_nlive(void);
int		hook_freed(const void *p);
size_t	hook_nfree(void);
void	hook_release(void);

/* Allocator that bypasses the hook (never fails, never tracked). */
void	*raw_malloc(size_t size);
void	raw_free(void *p);

/* Builds lists WITHOUT ft_lstnew, so each function is tested on its own. */
t_list	*mk_node(void *content);
t_list	*mk_list(void **contents, size_t n);
void	free_nodes(t_list *lst);

/* Length that stops at LIST_CAP (so a cycle does not hang the test). */
# define LIST_CAP 1000000
size_t	list_len(t_list *lst);

/* `del` that only records what it was given. */
# define DEL_LOG_MAX 4096
extern void		*g_del_log[DEL_LOG_MAX];
extern size_t	g_ndel;
void	rec_del(void *content);
int		del_was_called_with(const void *p);

#endif
