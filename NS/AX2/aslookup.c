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

struct as_info {
    int as_num;
    char *prefix;
    struct as_info * next;
};

struct hash_table {
    int size;
    struct as_info **table;
};

static struct hash_table *init_hash_table (int size) {
    struct hash_table *table;
    if ((table = malloc(sizeof(struct hash_table))) == NULL) {
	return NULL;
    }
    if ((table->table = malloc (sizeof(struct as_info *) * size)) == NULL) {
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

static unsigned int hash (struct hash_table *table, char *key) {
    int bucket_number = atoi(key);
    return bucket_number % table->size;
}

static struct as_info *create_pair (char *key, int value) {
    struct as_info *new_pair;
    if ((new_pair = malloc (sizeof (struct as_info))) == NULL) {
	return NULL;
    }
    if ((new_pair->prefix = strdup(key)) == NULL) {
	return NULL;
    }
    new_pair->as_num = value;
    new_pair->next = NULL;
    
    return new_pair;
} 

static int get_pair (struct hash_table *table, char *key) {
    struct as_info *pair;
    unsigned int hash_value = hash(table, key);
    pair = table->table[hash_value];
    while (pair != NULL && strcmp(pair->prefix, key) > 0 && pair->prefix != NULL) {
	pair = pair->next;
    }
    if (pair == NULL || pair->prefix == NULL || strcmp(pair->prefix, key) != 0) {
	return NULL;
    }
    else {
	return pair->as_num;
    } 
}

static void add_pair (struct hash_table *table, char *key, int value) {
    unsigned int hash_value = hash(table, key);
    struct as_info *new_pair = NULL;
    struct as_info *previous_pair = NULL;
    struct as_info *next_pair = NULL;

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
static struct as_info *extract_info (char *line) {
    struct as_info *info = malloc(sizeof(struct as_info));
    info->prefix = malloc (30);
    char *pref = malloc(30);
    int as_num;
    char *token;
    const char delim[2] = "|";
    const char delimSpace[2] = " ";
    
    token = strtok(line, delim);
    int i = 0;
    while (i < 5) {
	token = strtok(NULL, delim);
	++i;
    }
    strcpy(info->prefix, token);
    token = strtok(NULL, delim);
    int len = strlen(token);
    char *last = token + len - 1;
    char *num_char = malloc (30);
    while (*last != ' ' && *last != '|') {
        --last;
    }
    strcpy(num_char, last);
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

    struct as_info *info = malloc(sizeof(struct as_info));

    char *open_file = malloc (50);
    strcpy(open_file, "./bgpdump -Mv ");
    strcat(open_file, argv[1]);
    FILE *rib_file = popen(open_file, "r");

    struct hash_table *hashtable = init_hash_table(256);
    char *current = malloc(30);
    strcpy(current, (extract_info(line = fgets(line, LINE_SIZE, rib_file)))->prefix);
    char *previous = malloc(30);
    strcpy(previous, current);

    while ((line = fgets(line, LINE_SIZE, rib_file)) != NULL) {
        info = extract_info(line);
        strcpy(current, info->prefix);
	if (strcmp(current, previous) == 0) {
    	    strcpy(previous, current);
	    continue;
	}
        add_pair(hashtable, info->prefix, info->as_num);
	strcpy(previous, current);
    }
    
    free(info);
    free(current);
    free(previous);
    free(open_file);

    struct as *autnums = load_autnums();
    struct as_info *result = malloc(sizeof(struct as_info *));

    for (int i = 2; i < argc; ++i) {
        int row = atoi(argv[i]);
	int count = 0;
	char *previous_prefix = malloc(30);
        for(struct as_info* cursor = hashtable->table[row]; cursor != NULL; cursor = cursor->next) {
	    if (addr_matches_prefix(argv[i], cursor->prefix) == 1) {
		++count;
	        if ((count == 1) || (strcmp(cursor->prefix, previous_prefix) > 0)) {
	            strcpy(previous_prefix, cursor->prefix);
		    struct as *as_cursor = autnums;
		    while (as_cursor != NULL && as_cursor->num != cursor->as_num) {
			as_cursor = as_cursor->next;
	            }
		    if (as_cursor != NULL) {
		    	result->prefix = malloc(30);
			result->as_num = as_cursor->num;
			strcpy(result->prefix, as_cursor->name);
		    }
	        }
	    }
	}
	if (count == 1) {
	    printf("%s    %d    %s\n", argv[i], result->as_num, result->prefix);
	}
	else if (count == 0) {
	    printf("%s        - unknown\n", argv[i]);
	}
	else {
	    printf("%s        - multiple\n", argv[i]);
	}
    }

    free(autnums);
    for (int i=0; i < 256; ++i) {
	free(hashtable->table[i]);
    }
    free(hashtable);
    pclose(rib_file);

    return 0;
}
