/** 
  * Class for returning autonomous system number and name for all
  * IP addresses passed as command line arguments
  * GUID: 2147392n
  * NS(H) Assessed Exercise 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LINE_SIZE 1000

struct as {
    int num;
    char *name;
    struct as *next;
};

// struct for defining the hashtable node
struct node {
    int as_num;
    char *prefix;
    struct node * next;
};

// struct for the bucket hashtable used for storing prefixes with 
// corresponding AS numbers
struct hash_table {
    int size;
    struct node **table;
};

// return prefix's length
static int get_pref_len (char *prefix) {
    char *index;
    index = strchr(prefix, '/');
    ++index;
    int len = atoi(index);
    return len;
}

// initialize bucket-based hashtable
static struct hash_table *init_hash_table (int size) {
    struct hash_table *table;
    if ((table = malloc(sizeof(struct hash_table))) == NULL) {
        return NULL;
    }
    if ((table->table = malloc (sizeof(struct node *) * size)) == NULL) {
        return NULL;
    }
    if (size != 256) {
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        table->table[i] = NULL;
    }
    table->size = size;
    return table;
}

// hash function returning the correct bucket number for a prefix
static unsigned int hash (struct hash_table *table, char *key) {
    int bucket_number = atoi(key);
    return bucket_number % table->size;
}

// creates a string-integer node for the hashtable
static struct node *create_pair (char *key, int value) {
    struct node *new_pair;
    if ((new_pair = malloc (sizeof (struct node))) == NULL) {
        return NULL;
    }
    if ((new_pair->prefix = strdup(key)) == NULL) {
        return NULL;
    }
    new_pair->as_num = value;
    new_pair->next = NULL;

    return new_pair;
} 

// hashtable put function
static void add_pair (struct hash_table *table, char *key, int value) {
    unsigned int hash_value = hash(table, key);
    struct node *new_pair = NULL;
    struct node *previous_pair = NULL;
    struct node *next_pair = NULL;

    next_pair = table->table[hash_value];
    while (next_pair != NULL && strcmp (key, next_pair->prefix) == 0 && next_pair->prefix != NULL) {
        previous_pair = next_pair;
        next_pair = next_pair->next;
    }
    if(next_pair != NULL && next_pair->prefix != NULL && strcmp(key, next_pair->prefix) == 0 && next_pair->as_num == value) {
        return;
    } 
    new_pair = create_pair (key, value);
    if (next_pair == table->table[hash_value]) {
        new_pair->next = next_pair;
        table->table[hash_value] = new_pair;
    }
    else if (next_pair == NULL) {
        previous_pair->next = new_pair;
    }
    else {
        new_pair->next = next_pair;
        previous_pair->next = new_pair;
    }
}        


static struct as * load_autnums(void) {
    struct as *head = NULL;
    FILE *inf = fopen("autnums.html", "r");
    if (inf != NULL) {
        int buflen = 1024;
        char buffer[buflen+1];
        while (!feof(inf)) {
            memset(buffer, 0, buflen+1);
            fgets(buffer, buflen, inf);
            if (strncmp(buffer, "<a href=\"/cgi-bin/as-report=AS", 8) == 0) {
                int asnum;
                char asname[1024];
                char *p = "<a href=\"/cgi-bin/as-report?as=AS%*d&view=2.0\">AS%d </a> %[^\r\n]";
                if (sscanf(buffer, p, &asnum, asname) == 2) {
                    struct as *curr = malloc(sizeof(struct as));
                    curr->num = asnum;
                    curr->name = strdup(asname);
                    curr->next = head;
                    head = curr;
                }
            }
        }
        fclose(inf);
    }
    return head;
}

// function to extract the prefix and AS number from bgpdump output
static struct node *extract_info (char *line) {
    struct node *info = malloc(sizeof(struct node));
    info->prefix = NULL;
    char *token = NULL;

    token = strtok(line, "|");
    int i = 0;
    while (i < 5) {
        token = strtok(NULL, "|");
        ++i;
    }
    info->prefix = strdup(token);
    token = strtok(NULL, "|");
    int len = strlen(token);
    char *last = token + len - 1;	
    char *num_char = NULL; 
    while (*last != ' ' && *last != '|') {
        --last;
    }
    num_char = strdup(last);
    info->as_num = atoi(num_char);
    return info;
}

static void bytes_to_bitmap(int byte, char *bitmap) {
    int offset = 0;
    assert(byte >= 0);
    assert(byte <= 255);
    for (int i = 7; i >= 0; i--) {
        if ((byte & (1 << i)) != 0) {
            bitmap[offset++] = '1';
        } else {
            bitmap[offset++] = '0';
        }
    }
}

static int addr_matches_prefix(char *addr, char *prefix) {
    // Check if an IP prefix covers an IP address. The addr field is an IPv4
    // address in textual form (e.g., "130.209.240.1"); the prefix field has
    // its prefix length specified in the usual address/length format (e.g.,
    // "130.209.0.0/16"). Return 1 if the prefix covers the address, or zero
    // otherwise. This is not an efficient implementation, but is simple to
    // debug, since the bitmaps it uses for comparison are printable text.

    // Parse the address:
    int addr_bytes[4];
    char addr_bitmap[32+1];
    sprintf(addr_bitmap, "                                          ");
    if (sscanf(addr, "%d.%d.%d.%d",
                &addr_bytes[0], &addr_bytes[1], &addr_bytes[2], &addr_bytes[3]) != 4) {
        printf("cannot parse addr\n");
        return 0;
    }
    for (int i = 0; i < 4; i++) {
        bytes_to_bitmap(addr_bytes[i], &addr_bitmap[i * 8]);
    }
    // Parse the prefix:
    int prefix_bytes[4];
    int prefix_len;
    char prefix_bitmap[32+1];
    sprintf(prefix_bitmap, "                                   ");
    if (sscanf(prefix, "%d.%d.%d.%d/%d",
                &prefix_bytes[0], &prefix_bytes[1], &prefix_bytes[2], &prefix_bytes[3],
                &prefix_len) != 5) {
        printf("cannot parse prefix\n");
        return 0;
    }
    for (int i = 0; i < 4; i++) {
        bytes_to_bitmap(prefix_bytes[i], &prefix_bitmap[i * 8]);
    }

    for (int i = prefix_len + 1; i < 33; i++) {
        prefix_bitmap[i-1] = '?';
    }
    // Check if address matches prefix:
    for (int i = 0; i < 32; i++) {
        if ((addr_bitmap[i] != prefix_bitmap[i]) && (prefix_bitmap[i] != '?')) {
            return 0;
        }
    }
    return 1;
}

int main (int argc, char *argv[]) {  
    char *line = malloc (1000);

    struct node *info = malloc(sizeof(struct node));

    if (argc == 1) {
	printf("Please specify a rib file.\n");
	return -1;
    }

    // create the path for running bgpdump's process
    char *filename = argv[1];
    char *path = malloc(strlen("./bgpdump -Mv %s") + strlen(filename));
    sprintf(path, "./bgpdump -Mv %s", filename);
    FILE *rib_file = popen(path, "r");
    
    if (rib_file == NULL) {
	printf("Could not open rib file.\n");
	return -1;
    }

    // initialize the hashtable to store rib's contents
    struct hash_table *hashtable = init_hash_table(256);
    char *current = NULL;
    char *previous = strdup("previous_prefix");
    int i = 0;

    // loop through rib file's lines and extract information, adding
    // nodes to the hashtable
    while ((line = fgets(line, LINE_SIZE, rib_file)) != NULL) {
        info = extract_info(line);
        current = strdup(info->prefix);
        if (strcmp(previous, current) != 0) {
            free(previous);
            previous = strdup(current);
            add_pair(hashtable, info->prefix, info->as_num);
            strcpy(previous, current);
            if (i % 100000 == 0) {
                printf(".\n");
		fflush(stdout);
            }
            ++i;
        }
        free(current);
    }

    struct as *autnums = load_autnums();

    if (autnums == NULL ) {
        printf("Could not load autnums.html.\n");
        return -1;
    }

    if (argc <= 2) {
	printf("Please specify at least one IP address.\n");
	return -1;
    }

    // loop through each IP address passed as a command line argument and
    // find its AS number and AS name
    for (int i = 2; i < argc; ++i) {
        int row = atoi(argv[i]);
        char *address = strdup(argv[i]);
        int count = 0;
        struct node *match = NULL;
        struct node *cursor = hashtable->table[row];
        while (cursor != NULL) {
            if (addr_matches_prefix(address, cursor->prefix)) {
                if (match != NULL && get_pref_len(cursor->prefix) == get_pref_len (match->prefix)) {
                    count = 2;
                    break;
                }     		
                if (match == NULL || get_pref_len(cursor->prefix) > get_pref_len(match->prefix)) {
                    count = 1;
                    match = cursor;
                }
            }
            cursor = cursor->next;
        }

	// side comment: I would have formatted the string, but this cannot be done as I do not
	// know the maximum length of the AS numbers used in the rib files used for testing
        if (count == 0) {
            printf("%s  %s  %s\n", argv[i], "-", "unknown");
        }
        else if (count == 2) {
            printf("%s  %s  %s \n", argv[i], "-", "multiple");
        }
        else {
            int as_number = match->as_num;
            char *as_name = NULL;
            struct as *as_cursor = autnums;
            while (as_cursor != NULL) {
                if (as_cursor->num == as_number) {
                    as_name = as_cursor->name;
                    break;
                } 
                as_cursor = as_cursor->next;
            }
            if (as_cursor != NULL) {
                printf("%s  %d  %s\n", argv[i], as_number, as_name);
            }
            else {
                printf("%s  %s  %s\n", argv[i], "-", "unknown");
            }
        }
    }

    // avoid any memory leaks by correctly freeing all allocated memory
    free(line);
    free(info);
    free(current);
    free(previous);
    free(path);
    struct as *cursor = autnums;
    while (cursor != NULL) {
        struct as *delete_node = cursor;
        cursor = cursor->next;
        free(delete_node->name);
        free(delete_node);
    }
    for (int i=0; i < 256; ++i) {
        struct node *cursor = hashtable->table[i];
        while(cursor != NULL) {
            struct node *delete_node = cursor;
            cursor = cursor->next;
            free(delete_node->prefix);
            free(delete_node);
        }
    }
    free(hashtable);
    pclose(rib_file);

    return 0;
}
