/* llist.c */

#include "llist_threads.h"
#define TRUE 1
#define FALSE -1

/* Right now, this routine simply ensures that we don't initialize a list that has data on it. */

int llist_init(llist_t *llistp)
{
	llistp->first = NULL;
	pthread_mutex_init(&(llistp->mutex), NULL);
	return 0;
}

int llist_insert_data(int index, void *datap, llist_t *llistp)
{
	llist_node_t *cur, *prev, *new;
	int found = FALSE;
	
	pthread_mutex_lock(&(llistp->mutex));

	for(cur = prev = *llistp; cur != NULL; prev = cur, cur = cur->nextp) {
		if(cur->index == index) {
			free(cur->datap);
			cur->datap = datap;
			found = TRUE;
			break;
		} else if (cur->index > index) {
			break;
		}
	}

	if(!found) {
		new = (llist_node_t *)malloc(sizeof(llist_node_t));
		new->index = index;
		new->nextp = cur;
		if(cur == llistp->first)
			llistp->first = new;
		else
			prev->nextp = new;
	}
	
	pthread_mutex_unlock(&(llistp->mutex));

	return 0;
}
