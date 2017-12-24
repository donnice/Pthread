#include <stdio.h>
#include <stdlib.h>

/* list.h */
typedef struct list_node {
	int					index;
	void				*datap;
	struct llist_node	*nextp;
} llist_node_t;

typedef llist_node_t *llist_t;

int llist_init(llist_t *llistp);
int llist_insert_data(int index, void *datap, llist_t *llistp);
int llist_remove_data(int index, void **datapp, llist_t *llistp);
int llist_find_data(int index, void **datapp, llist_t *llistp);
int llist_show(llist_t *llistp);
