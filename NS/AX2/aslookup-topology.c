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

static int get_pref_len (char *prefix) {
    char *index = NULL;
    index = strchr(prefix, '/');
    ++index;
    int len = atoi(index);
    return len;
}

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
    struct as_info *info = malloc(sizeof(struct as_info));

    char *filename = argv[1];
    char *path = malloc(strlen("./bgpdump -Mv %s") + strlen(filename));
    sprintf(path, "./bgpdump -Mv %s", filename);
    FILE *rib_file = popen(path, "r");
    struct hash_table *hashtable = init_hash_table(256);
    char *current = NULL;
    current = strdup(extract_info(line = fgets(line, LINE_SIZE, rib_file))->prefix);
    char *previous = strdup(current);

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

    pclose(rib_file);    

    struct as *autnums = load_autnums();

    FILE *txt_file = fopen("router-topology.txt", "r");
    FILE *topology = fopen("as-topology.txt", "w+");

    fflush(topology);
    fprintf(topology, "graph astopology {\n");

    char *line_one = malloc(1000);

    while ((line_one = fgets(line_one, LINE_SIZE, txt_file)) != NULL) {
	fflush(topology);
	if (strchr(line_one, '}') != NULL || strchr(line_one, '{') != NULL ) {
	    continue;
	}
	char *token = NULL;
	char *address_one = NULL;
	char *address_two = NULL;
	token = strtok(line_one+1, "\"");
	int i = 0;
	while (token != NULL) {
	    if (i==0) {
	        address_one = strdup(token);
	    }
	    else if (i==2) {
		address_two = strdup(token);
	    }
	    token = strtok(NULL, "\"");
	    ++i;
	}
	int row = atoi(address_one);
	int count = 0;
        struct as_info *match = NULL;
	struct as_info *cursor = hashtable->table[row];
	while (cursor != NULL) {
            if (addr_matches_prefix(address_one, cursor->prefix)) {
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
	if (count == 0) {
            fprintf(topology, "\"- unknown\" -- ");
	}
        else if (count == 2) {
            fprintf(topology, "\"- multiple\" -- ");
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
                fprintf(topology, "\"%d %s\" -- ", as_number, as_name);
            }
	    else {
                fprintf(topology, "\"- unknown\" -- ");
            }
        }
	row = atoi(address_two);
	count = 0;
        match = NULL;
	cursor = hashtable->table[row];
	while (cursor != NULL) {
            if (addr_matches_prefix(address_two, cursor->prefix)) {
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
	if (count == 0) {
            fprintf(topology, "\"- unknown\"\n");
	}
        else if (count == 2) {
            fprintf(topology, "\"- multiple\"\n");
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
                fprintf(topology, "\"%d %s\"\n", as_number, as_name);
            }
	    else {
                fprintf(topology, "\"- unknown\"\n");
            }
        }

            
    }

    fflush(topology);
    fprintf(topology, "}");
    fclose(txt_file);
    fclose(topology);
    free(line);
    free(info);
    free(current);
    free(previous);
    free(path);
    free(autnums);
    for (int i=0; i < 256; ++i) {
        free(hashtable->table[i]);
    }
    free(hashtable);

    return 0;
}
