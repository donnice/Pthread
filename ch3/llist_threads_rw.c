/* llist.c */

#include "llist_threads_rw.h"
#define TRUE 1
#define FALSE -1

/* Right now, this routine simply ensures that we don't initialize a list that has data on it. */

int llist_init(llist_t *llistp)
{
	llistp->first = NULL;
	pthread_rwdr_init_np(&(llistp->rwlock), NULL);
	return 0;
}

int llist_insert_data(int index, void *datap, llist_t *llistp)
{
	llist_node_t *cur, *prev, *new;
	int found = FALSE;
	
	pthread_rwdr_wlock_up(&(llistp->rwlock));

	for(cur = prev = llistp->first; cur != NULL; prev = cur, cur = cur->nextp) {
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
	
	pthread_rwdr_wunlock_np(&(llistp->rwlock));

	return 0;
}

int llist_find_data(int index, void *datap, llist_t *llistp)
{

	llist_node_t *cur, *prev, *new;
	int found = FALSE;

	pthread_rwdr_rlock_up(&(llistp->rwlock));

	for(cur = prev = llistp->first; cur != NULL; prev = cur, cur = cur->nextp) {
		if(cur->index == index) {
			free(cur->datap);
			cur->datap = datap;
			found = TRUE;
			break;
		} else if (cur->index > index) {
			break;
		}
	}

	pthread_rwdr_runlock_np(&(llistp->rwlock));

	if(found) {
		printf("Found index: %d", index);
		return index;
	}

	return -1;
}

