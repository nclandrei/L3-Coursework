#ifndef _TSHTABLE_H_
#define _TSHTABLE_H_

/*
 * interface to thread-safe, generic hash table
 * associates a key string with a void *datum
 *
 * provides an iterator, use model of iterator is as follows:
 * 	iter_create - acquires lock on hash table
 * 	iter_next - assumes lock is held on hash table
 * 	iter_delete - releases lock on hash table
 */

typedef struct tshtable TSHTable;
typedef struct htiterator HTIterator;
typedef struct htitervalue {
   char *key;
   void *datum;
}HTIterValue;

/*
 * tsht_create - created a new hashtable
 *             size is the number of buckets; if 0, use default
 */
TSHTable *tsht_create(unsigned long size);

/*
 * tsht_delete - delete the hashtable
 *             deletes the hashtable iff it is empty
 *             returns 1 if successful, 0 if not empty
 */
int tsht_delete(TSHTable *ht);

/*
 * tsht_insert - inserts a new hashtable entry for (key, datum) into table
 *             returns 1 if successful, 0 if malloc error
 *             if already an entry for key, inserts new datum, and
 *             sets old to old datum; otherwise, sets old to NULL
 */
int tsht_insert(TSHTable *ht, char *key, void *datum, void **old);

/*
 * tsht_lookup - looks for key in the hashtable
 *             returns associated datum or NULL
 */
void *tsht_lookup(TSHTable *ht, char *key);

/*
 * tsht_remove - removes associated key from the table, if found
 *             returns 1 if found key and sets datum to associated value
 *             returns 0 if not
 */
int tsht_remove(TSHTable *ht, char *key, void **datum);

/*
 * tsht_keys - return pointers to all of the keys defined in the table
 *           returns number of pointers in the array, -1 if malloc failure
 */
int tsht_keys(TSHTable *ht, char ***theKeys);

/*
 * tsht_iter_create - creates an iterator for running through the hash table
 */
HTIterator *tsht_iter_create(TSHTable *ht);

/*
 * tsht_iter_next - returns pointer to the HTIterValue of the next element in
 *                the hash table or NULL if the hash table is exhausted
 */
HTIterValue *tsht_iter_next(HTIterator *iter);

/*
 * tsht_iter_delete - delete the iterator
 */
void tsht_iter_delete(HTIterator *iter);

#endif /* _TSHTABLE_H_ */
