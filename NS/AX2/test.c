#include<stdio.h>
#include<string.h>
#include<stdlib.h> 
#include <assert.h>


struct as{
  int num;
  char *name;
  struct as *next;  
};

struct hashnode{
    char *prefix;
    char *number;
    struct hashnode *next;
  
};

static void
bytes_to_bitmap(int byte, char *bitmap)
{
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

static int 
get_prefix_length(char *addr){
  
  char *p;
  p=strrchr(addr,'/');
  p++;
  int m=atoi(p);
  return m;
  
  
}

static int
addr_matches_prefix(char *addr, char *prefix) {
// Check if an IP prefix covers an IP address. The addr field is an IPv4
// address in textual form (e.g., "130.209.240.1"); the prefix field has
// its prefix length specified in the usual address/length format (e.g.,
// "130.209.0.0/16"). Return 1 if the prefix covers the address, or zero
// otherwise. This is not an efficient implementation, but is simple to
// debug, since the bitmaps it uses for comparison are printable text.

// Parse the address:
  int  addr_bytes[4];
  char addr_bitmap[32+1];
  sprintf(addr_bitmap, "    ");
  if (sscanf(addr, "%d.%d.%d.%d",&addr_bytes[0], &addr_bytes[1], &addr_bytes[2], &addr_bytes[3]) != 4) {
    printf("cannot parse addr\n");
    return 0;
  }

  for (int i = 0; i < 4; i++) {
    bytes_to_bitmap(addr_bytes[i], &addr_bitmap[i*8]);
  }
// Parse the prefix:
  int  prefix_bytes[4];
  int  prefix_len;
  char prefix_bitmap[32+1];
  sprintf(prefix_bitmap, "    ");
  if (sscanf(prefix, "%d.%d.%d.%d/%d",&prefix_bytes[0], &prefix_bytes[1], &prefix_bytes[2], &prefix_bytes[3],&prefix_len) != 5) {
    printf("cannot parse prefix\n");
    return 0;
  }
  for (int i = 0; i < 4; i++) {
    bytes_to_bitmap(prefix_bytes[i], &prefix_bitmap[i*8]);
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



static struct as *load_autnums(void)
{
  struct as*head = NULL;
  FILE *inf  = fopen("autnums.html", "r");
  if (inf != NULL) {
    int  buflen = 1024;
    char buffer[buflen+1];
    while (!feof(inf)) {
      memset(buffer, 0, buflen+1);
      fgets(buffer, buflen, inf);
      if (strncmp(buffer, "<a href=\"/cgi-bin/as-report=AS", 8) == 0) {
	int   asnum;
	char  asname[1024];
	char *p = "<a href=\"/cgi-bin/as-report?as=AS%*d&view=2.0\">AS%d </a> %[^\r\n]";
	if (sscanf(buffer, p, &asnum, asname) == 2) {
	  struct as *curr = malloc(sizeof(struct as));
	  curr->num  = asnum;
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




int main(int argc, char *argv[]){
    struct hashnode *hashmap[256]={NULL};
    //opening the file
    struct as* isp;
    char *filename=argv[1];
    char *path=malloc(sizeof(char)*strlen("./bgpdump -Mv %s")+sizeof(char)*strlen(filename));
    sprintf(path,"./bgpdump -Mv %s",filename);
    FILE *rib_file = popen(path, "r");
    char *prev_prefix=NULL;
    prev_prefix=strdup("p");
    ssize_t read;
    size_t len = 0;
    char *line=NULL;
    
     while ((read = getline(&line, &len, rib_file)) != -1) {
	char *pref=NULL;
	char *num=NULL;
	int i=0;
	char *num_cursor;
	char *cursor=strtok(line,"|");
	while(cursor!=NULL){            //extract prefix and number
	  i++;
	  if(i==6){
	    pref=strdup(cursor);
	  }
	  if(i==7){
	    num_cursor=strrchr(cursor,' ');
	    if(num_cursor!=NULL)
	      num_cursor++;
	    else
	      num_cursor=cursor;
	   num=strdup(num_cursor);
	  }
	  cursor=strtok(NULL,"|");
	}
	

	if(strcmp(prev_prefix,pref)!=0){
	  int index=atoi(pref);
	  free(prev_prefix);
	  prev_prefix=strdup(pref);
	  struct hashnode *hn=malloc(sizeof(struct hashnode));
	  if(hn!=NULL){
	      hn->prefix=strdup(pref);
	      hn->number=strdup(num);
	      hn->next=NULL;
	  }
	  
	  if(hashmap[index]==NULL){
	      hashmap[index]=hn;
	  }
	  
	  else{
	      struct hashnode *current=hashmap[index];
	      hn->next=current;
	      hashmap[index]=hn;
	  }
	  
	}
	free(pref);
	free(num);
	

    }
    
    

    isp=load_autnums();
    int i;
    for(i=2;i<argc;i++){
	char *address=argv[i];
	int index=atoi(address);
	struct hashnode *current=hashmap[index];
	struct hashnode *match=NULL;
	while(current!=NULL){
	    if(addr_matches_prefix(address,current->prefix)){
		if((match==NULL) ||(get_prefix_length(match->prefix)<get_prefix_length(current->prefix)))
		    match=current;
	    }
	    current=current->next;
	   
	}
	if(match==NULL){
	    fprintf(stderr,"Error:No match found\n");
	    return -1;
	}
	else{
	    int as_number=atoi(match->number);
	    struct as *curr=isp;
	    char *as_name=NULL;
	    while(curr!=NULL){
	      if(curr->num==as_number){
		as_name=curr->name;
		break;
	      }
	      else{
		curr=curr->next;
	      }
	    }
	    if(as_name==NULL)
	      printf("No as fouund\n");
	    else
	      printf("%s %d %s\n",address,as_number,as_name);
	}
    }
    for(i=0;i<=255;i++){
        struct hashnode *to_delete=hashmap[i];
	while(to_delete!=NULL){
	    struct hashnode *elem1=to_delete;
	    to_delete=to_delete->next;
	    free(elem1->prefix);
	    free(elem1->number);
	    free(elem1);
	}
      
    }
     struct as *curr=isp;
     while(curr!=NULL){
       struct as *to_delete=curr;
       curr=curr->next;
       free(to_delete->name);
       free(to_delete);
     }
     pclose(rib_file);
     free(path);
     free(prev_prefix);
     free(line);
    
   
   
    return 0;
}
  
