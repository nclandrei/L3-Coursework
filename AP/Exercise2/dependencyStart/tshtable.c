#include "tshtable.h"
#include "mem.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define DEFAULT_SIZE 1024l

/*
 * private structures used to represent a hash table and table entries
 */
typedef struct h_entry {
	struct h_entry *next;
	char *key;
	void *datum;
} H_Entry;

typedef struct h_node {
	H_Entry *first;
} H_Node;

struct tshtable {
	pthread_mutex_t lock;
	unsigned long size;
	unsigned long nelements;
	H_Node *table;
};

struct htiterator {
	TSHTable *ht;
	long ndx;
	H_Entry *entry;
	HTIterValue htiv;
};

/*
 * generate hash value from s; value returned in range 0..N-1
 */
#define SHIFT 7l		/* should be prime */
static unsigned long gen_hash(char *s, unsigned long N) {
	unsigned long hash;
	char *sp;

	hash = 0l;
	for (sp = s; *sp != '\0'; sp++)
		hash = ((SHIFT * hash) + *sp) % N;
	return hash;
}

TSHTable *tsht_create(unsigned long size) {
	TSHTable *t;
	unsigned long N;
	H_Node *array;
	unsigned long i;

	t = (TSHTable *)mem_alloc(sizeof(TSHTable));
	if (t != NULL) {
		N = ((size > 0) ? size : DEFAULT_SIZE);
		array = (H_Node *)mem_alloc(N * sizeof(H_Node));
		if (array != NULL) {
			t->size = N;
			t->nelements = 0;
			t->table = array;
			for (i = 0l; i < N; i++)
				array[i].first = NULL;
			pthread_mutex_init(&(t->lock), NULL);
		} else {
			mem_free((void *)t);
			t = NULL;
		}
	}
	return t;
}

int tsht_delete(TSHTable *ht) {
	int ans = 0;
	pthread_mutex_t *m = &(ht->lock);

	pthread_mutex_lock(m);
	if (ht->nelements == 0) {
		ans = 1;
	}
	pthread_mutex_unlock(m);
	if (ans) {
		mem_free(ht->table);
		mem_free(ht);
	}
	return ans;
}

int tsht_insert(TSHTable *ht, char *str, void *datum, void **old) {
	unsigned long hash;
	H_Entry *p;

	pthread_mutex_lock(&(ht->lock));
	hash = gen_hash(str, ht->size);
	p = ht->table[hash].first;
	while (p != NULL) {
		if (strcmp(str, p->key) == 0) {
			*old = p->datum;
		 p->datum = datum;
			pthread_mutex_unlock(&(ht->lock));
		 return 1;
		}
		p = p->next;
	}
	p = (H_Entry *)mem_alloc(sizeof(H_Entry));
	if (p != NULL) {
		char *s = strdup(str);
		if (s == NULL) {
			mem_free((void *)p);
			pthread_mutex_unlock(&(ht->lock));
			return 0;
		}
		p->key = s;
		p->datum = datum;
		p->next = ht->table[hash].first;
		ht->table[hash].first = p;
		ht->nelements++;
		pthread_mutex_unlock(&(ht->lock));
		*old = NULL;
		return 1;
	}
	pthread_mutex_unlock(&(ht->lock));
	return 0;
}

void *tsht_lookup(TSHTable *ht, char *str) {
	unsigned long hash;
	H_Entry *p;

	pthread_mutex_lock(&(ht->lock));
	hash = gen_hash(str, ht->size);
	p = ht->table[hash].first;
	while (p != NULL) {
		if (strcmp(str, p->key) == 0) {
			pthread_mutex_unlock(&(ht->lock));
			return p->datum;
		}
		p = p->next;
	}
	pthread_mutex_unlock(&(ht->lock));
	return NULL;
}

int tsht_remove(TSHTable *ht, char *str, void **datum) {
	unsigned long hash;
	H_Entry *p, *q;
	int found = 0;

	pthread_mutex_lock(&(ht->lock));
	hash = gen_hash(str, ht->size);
	p = ht->table[hash].first;
	q = NULL;
	while (p != NULL) {
		if (strcmp(str, p->key) == 0) {
			found++;
			break;
		}
		q = p;
		p = q->next;
	}
	if (found) {
		*datum = p->datum;
		if (q == NULL)
			ht->table[hash].first = p->next;
		else
			q->next = p->next;
		ht->nelements--;
		mem_free((void *)p->key);
		mem_free((void *)p);
	}
	pthread_mutex_unlock(&(ht->lock));
	return found;
}

int tsht_keys(TSHTable *ht, char ***theKeys) {
	int ans = ht->nelements;
	if (! ans) {		/* an empty table */
		*theKeys = NULL;
	} else {
		char **keys = (char **)mem_alloc(ans * sizeof(char *));
		if (! keys) {
			ans = -1;
		} else {
			unsigned long i;
			int n = 0;
			H_Entry *p;

			pthread_mutex_lock(&(ht->lock));
			for (i = 0; i < ht->size; i++) {
				p = ht->table[i].first;
				while (p != NULL) {
					keys[n++] = p->key;
					p = p->next;
				}
			}
			pthread_mutex_unlock(&(ht->lock));
		}
		*theKeys = keys;
	}
	return ans;
}

/*
 * note - in order to maintain thread safety, create acquires the lock on the
 *        table, delete releases it; thus, the usage pattern supported
 *        by these routines is for a single thread to
 *
 *        1. create the iterator
 *        2. iterate through the table elements
 *        3. destroy the iterator
 *
 *        during which time the table is locked to prevent access by other
 *        threads.
 */
HTIterator *tsht_iter_create(TSHTable *ht) {
	HTIterator *iter = (HTIterator *)mem_alloc(sizeof(HTIterator));
	if (iter) {
		iter->ht = ht;
	iter->ndx = -1;
	iter->entry = NULL;
	(iter->htiv).key = NULL;
	(iter->htiv).datum = NULL;
		pthread_mutex_lock(&(ht->lock));
	}
	return iter;
}

HTIterValue *tsht_iter_next(HTIterator *iter) {
	if (iter->entry != NULL)
		iter->entry = iter->entry->next;
	if (iter->entry == NULL) {
		unsigned long i;
		for (i = (unsigned long)(iter->ndx + 1); i < iter->ht->size; i++)
			if (iter->ht->table[i].first) {
				iter->ndx = i;
				iter->entry = iter->ht->table[i].first;
				(iter->htiv).key = iter->entry->key;
				(iter->htiv).datum = iter->entry->datum;
				return &(iter->htiv);
	 }
	} else {
		(iter->htiv).key = iter->entry->key;
		(iter->htiv).datum = iter->entry->datum;
		return &(iter->htiv);
	}
	return NULL;
}

void tsht_iter_delete(HTIterator *iter) {
	pthread_mutex_unlock(&(iter->ht->lock));
	mem_free((void *)iter);
}
