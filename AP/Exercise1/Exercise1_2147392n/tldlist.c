//
// Name: Andrei-Mihai Nicolae
// GUID: 2147392N
// Title of assignment: AP3 Exercise 1
// This is my own work as defined in the Academic Ethics agreement I have signed.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "date.h"
#include "tldlist.h"

struct tldlist {
    struct tldnode *root;
    long count;
    long size;
    Date *begin;
    Date *end;
};

struct tldnode {
    char *key;
    struct tldnode *left;
    struct tldnode *right;
    long height;
    long count;
};

struct tlditerator {
    struct tldlist *tldlist;
    int i;
    long size;
    struct tldnode **nodes;
};

/**
 * utility functions to improve the readability of the AVL tree insertion
 */

long height (struct tldnode *node) {
    if (node == NULL)
        return 0;
    return node->height;
};

long getBalance (struct tldnode *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
};

long max (long a, long b) {
    return (a > b)? a : b;
};

/**
 * the 2 rotation functions we are using for balancing the tree
 */

struct tldnode *leftRotation (struct tldnode *node1) {
    struct tldnode *node2 = node1->right;
    struct tldnode *node3 = node2->left;

    node2->left = node1;
    node1->right = node3;

    node1->height = max(height(node1->left), height(node1->right))+1;
    node2->height = max(height(node2->left), height(node2->right))+1;

    return node2;
};

struct tldnode *rightRotation (struct tldnode *node1) {
    struct tldnode *node2 = node1->left;
    struct tldnode *node3 = node2->right;

    node2->right = node1;
    node1->left = node3;

    node1->height = max(height(node1->left), height(node1->right))+1;
    node2->height = max(height(node2->left), height(node2->right))+1;


    return node2;
};

// this is the method of actually inserting a node that will be called from tldlist_add
struct tldnode *tldlist_add_node(TLDList *tldlist, TLDNode *root, TLDNode *node) {
    if (root == NULL) {
        ++tldlist->size;
        return node;
    }
    if (strcmp(node->key,root->key) < 0)
        root->left  = tldlist_add_node(tldlist, root->left, node);
    else if (strcmp(node->key, root->key) > 0)
        root->right = tldlist_add_node(tldlist, root->right, node);
    else {
        ++root->count;
    }

    root->height = max(height(root->left), height(root->right)) + 1;

    long balance = getBalance(root);
    if (balance > 1 && strcmp(node->key, root->left->key) < 0)
        return rightRotation(root);

    if (balance < -1 && strcmp(node->key, root->right->key) > 0)
      return leftRotation(root);

    if (balance > 1 && strcmp(node->key, root->left->key) > 0) {
        root->left =  leftRotation(root->left);
        return rightRotation(root);
    }
    if (balance < -1 && strcmp(node->key, root->right->key) < 0) {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }
    return root;
};

int tldlist_add(TLDList *tld, char *hostname, Date *d) {

    // if it is outside the interesting dates, we return 0
    if (date_compare(tld->begin, d) > 0 || date_compare(tld->end, d) < 0)
        return 0;

    // we now get the TLD
    char *tldString;
    char *tld_name;
    long len;

    tldString = strrchr(hostname, '.') + 1;
    len = strlen(tldString);
    tld_name = (char *)malloc(sizeof(char)*(len+1));
    tld_name[len] = '\0';
    strcpy(tld_name, tldString);

    TLDNode *node = (TLDNode *) malloc (sizeof(TLDNode));

    if (node != NULL) {
        node->key = tld_name;
        node->count = 1;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
        tld->root = tldlist_add_node(tld, tld->root, node);
        ++tld->count;
        return 1;
    }
    else
        return 0;
};

//create the tldlist using begin and end dates
struct tldlist *tldlist_create(Date *begin, Date *end) {
    TLDList *tldList = (TLDList *) malloc (sizeof(TLDList));
    if (tldList != NULL) {
        tldList->root = NULL;
        tldList->begin = date_duplicate(begin);
        tldList->end = date_duplicate(end);
        tldList->count = 0;
        tldList->size = 0;
    }
    return tldList;
};

// recursively free each node in the tree starting from the root
void tldnode_destroy (TLDNode *node) {
    if (node != NULL) {
        tldnode_destroy(node->left);
        tldnode_destroy(node->right);
        free(node);
    }
};

// destroys the tldlist
void tldlist_destroy(TLDList *tld) {
    if (tld != NULL)
        tldnode_destroy(tld->root);
};

// returns the number of nodes in the tldlist
long tldlist_count(TLDList *tld) {
    if (tld != NULL)
        return tld->count;
    else
        return 0;
};

// utility function called by populate, which will recursively add nodes from
// the AVL tree to the array of nodes we have in the iterator
void tldlist_iter_populate_recursive (TLDIterator *iter, TLDNode *node, int *index) {
    if (node->left != NULL)
        tldlist_iter_populate_recursive(iter, node->left, index);
	
	*(iter->nodes + (*index)++) = node;

    if (node->right != NULL)
        tldlist_iter_populate_recursive (iter, node->right, index);
};

void tldlist_iter_populate(TLDIterator *iter) {
    int index = 0;
	
	// we are doing this for the case when the user introduces 2 dates
	// between we have no tld nodes
	if (iter->size != 0)
		tldlist_iter_populate_recursive (iter, iter->tldlist->root, &index);
};

// returns the next node in the array
TLDNode *tldlist_iter_next(TLDIterator *iter) {
    if (iter->i == iter->size)
        return NULL;

    return *(iter->nodes + iter->i++);
};

// creates the iterator and populates it
TLDIterator *tldlist_iter_create(TLDList *tld) {
    TLDIterator *iterator = (TLDIterator *) malloc (sizeof(TLDIterator));
    if (iterator == NULL)
        return NULL;
   
    iterator->tldlist = tld;
    iterator->size = tld->size;
    iterator->i = 0;

    iterator->nodes = (TLDNode **)malloc(sizeof(TLDNode *) * iterator->size);

    if (iterator->nodes == NULL) {
        tldlist_iter_destroy(iterator);
        return NULL;
    }

    tldlist_iter_populate(iterator);

    return iterator;
};

// destroys the iterator freeing the array of nodes at first
// and then the actual iterator
void tldlist_iter_destroy(TLDIterator *iter) {
    free(iter->nodes);
    free(iter);
};

// returns the tld name inside the node
char *tldnode_tldname(TLDNode *node) {
    return node->key;
};

// counts the number of apparitions of that specific TLD
long tldnode_count(TLDNode *node) {
    if (node != NULL)
        return node->count;
    else
        return 0;
};
