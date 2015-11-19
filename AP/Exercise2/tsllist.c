#include "tsllist.h"
#include "mem.h"
#include <stdlib.h>
#include <pthread.h>

struct lnode {
	struct lnode *next;
	void *element;
};

struct llist {
	pthread_mutex_t lock;
	struct lnode *head;
	struct lnode *tail;
	unsigned long nelements;
};

struct lliterator {
	LList *ll;
	unsigned long next;
	unsigned long size;
	void **values;
};

/* constructor */
LList *ll_create(void) {
	LList *list = (LList *)mem_alloc(sizeof(LList));

	if (list != NULL) {
		list->head = NULL;
		list->tail = NULL;
		list->nelements = 0L;
		pthread_mutex_init(&(list->lock), NULL);
	}
	return list;
}

/* add element to the list, either at the head or at the tail
   returns 1 if successful, 0 if unsuccessful */
int ll_add_to_head(LList *list, void *element) {
	int status = 0;
	struct lnode *p;

	pthread_mutex_lock(&(list->lock));
	p = (struct lnode *)mem_alloc(sizeof(struct lnode));
	if (p != NULL) {
		status = 1;
		p->element = element;
		p->next = list->head;
		list->head = p;
		if (! list->nelements++)
			list->tail = p;
	}
	pthread_mutex_unlock(&(list->lock));
	return status;
}

int ll_add_to_tail(LList *list, void *element) {
	int status = 0;
	struct lnode *p;

	pthread_mutex_lock(&(list->lock));
	p = (struct lnode *)mem_alloc(sizeof(struct lnode));
	if (p != NULL) {
		status = 1;
		p->element = element;
		p->next = NULL;
		if (list->nelements++)
			list->tail->next = p;
		else
			list->head = p;
		list->tail = p;
	}
	pthread_mutex_unlock(&(list->lock));
	return status;
}

/* remove element from the head of the list
   returns pointer to element if successful, NULL if unsuccessful */
void *ll_remove_from_head(LList *list) {
	struct lnode *p;
	void *result = NULL;

	pthread_mutex_lock(&(list->lock));
	if (list->nelements) {
		p = list->head;
		list->head = p->next;
		if (! --list->nelements)
			list->tail = NULL;
		result = p->element;
		mem_free((void *)p);
	}
	pthread_mutex_unlock(&(list->lock));
	return result;
}

/* return the number of elements in the list */
unsigned long ll_nelements(LList *list) {
	unsigned long ans;

	pthread_mutex_lock(&(list->lock));
	ans = list->nelements;
	pthread_mutex_unlock(&(list->lock));
	return ans;
}

/* create an iterator over the linked list; returns NULL if unsuccessful */
/*
 * note - in order to maintain thread safety, create acquires the lock on the
 *        list, delete releases it; thus, the usage pattern supported
 *        by these routines is for a single thread to
 *
 *        1. create the iterator
 *        2. iterate through the list elements
 *        3. destroy the iterator
 *
 *        during which time the list is locked to prevent access by other
 *        threads.
 */
LLIterator *ll_iter_create(LList *list) {
	LLIterator *it = (LLIterator *)mem_alloc(sizeof(LLIterator));
	if (it != NULL) {
		pthread_mutex_lock(&(list->lock));
		it->next = 0;
		it->size = list->nelements;
		it->ll = list;
		if (it->size > 0) {
			it->values = (void **)mem_alloc(it->size*sizeof(void *));
			if (it->values == NULL) {
				mem_free(it);
				it = NULL;
			} else {
				struct lnode *p = list->head;
				unsigned long i;
				for (i = 0; i < list->nelements; i++) {
					it->values[i] = p->element;
					p = p->next;
				}
			}
		} else {
			it->values = NULL;
		}
	}
	if (it == NULL)
		pthread_mutex_unlock(&(list->lock));
	return it;
}

/* obtain the next element from the iterator; returns NULL if no more */
void *ll_iter_next(LLIterator *it) {
	void *ans = NULL;

	if (it->next < it->size) {
		ans = it->values[it->next];
		it->next++;
	}
	return ans;
}

/* delete the iterator */
void ll_iter_delete(LLIterator *it) {
	pthread_mutex_unlock(&(it->ll->lock));
	if (it->values != NULL)
		mem_free(it->values);
	mem_free(it);
}
