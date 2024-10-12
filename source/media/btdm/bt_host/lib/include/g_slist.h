#ifndef _G_LIST_H
#define G_LIST_H

/*
typedef struct _GSLIST GList;
typedef struct _GSLIST GSList;
struct _GSLIST {
	GSList *next;
	void *data;
};
*/

/*
 * singly-linked lists (after openobex implementation)
 */
static GSList *g_slist_append(GSList *p, void *d)
{
	GSList *q, *n = otk_malloc(sizeof(GSList));

	if (!n)
		return NULL;

	n->data = d;
	n->next = 0;

	if (!p)
		return n;

	for (q = p; q->next; q = q->next);
	q->next = n;

	return p;
}

static GSList *g_slist_remove(GSList *list, void *d)
{
	GSList *p, *q;

	for (q = 0, p = list; p; q = p, p = p->next)
		if (p->data == d) {
			if (q)
				q->next = p->next;
			else
				list = p->next;
			otk_free(p);
			break;
		}

	return list;
}

/*
 * Every element of the list points to things which need
 * to be free()'d. This method frees the list's contents
 */
typedef void(*g_slist_free_func_t)(void *);

static void g_slist_free_full(GSList *list, g_slist_free_func_t f)
{
	GSList *next;
	while (list) {
		next = list->next;
		if (f)
			f(list->data);
		otk_free(list);
		list = next;
	}
}


#endif	//G_LIST_H
