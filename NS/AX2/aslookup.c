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

/*static struct as_info *extract_info (char *line) {
    struct as_info *info = malloc(sizeof(struct as_info));
    char *pref = malloc(15);
    int as_num;
    printf("am ajuns aici\n");
    sscanf(line, "TABLE_DUMP2|%*s %*s|%*s|%*s|%*s|%s|%*d %*d %d|%*s", pref, &as_num);
    info->prefix = pref;
    info->as_num = as_num;
    printf("%s --- %d\n", pref, as_num);

    return info;
}
*/

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
    printf("aaa");
    struct as *autnums = load_autnums();
    //struct as_info *info = malloc(sizeof(struct as_info));

    char *open_file = malloc (50);
    strcpy(open_file, "./bgpdump -Mv ");
    strcat(open_file, argv[1]);
    printf("%s \n", open_file);
    FILE *rib_file = popen(open_file, "r");

    while ((line = fgets(line, LINE_SIZE, rib_file)) != NULL) {
        //info = extract_info(line);
        printf("%s \n", line);
/*            if prefix != previous prefix {
                save prefix and AS number in hash table, indexed by first octet of prefix
            }
    */
        //printf("%s --- %d \n", info->prefix, info->as_num);
    }/*
    load AS number to AS name mapping file (autnums.html)
        foreach address {
            foreach prefix in appropriate row of hash table {
                if address matches prefix {
                    if (first match for this prefix) or (longer prefix than previous match) {
                        store corresponding AS number and AS name
                    }
                }
            }
            print address, and matching AS number and AS name
                or print error if not found
                or print error if found multiple times
        }*/
    pclose(rib_file);
    return 0;
}
