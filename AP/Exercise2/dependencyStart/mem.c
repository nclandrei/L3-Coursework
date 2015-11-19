/* mem.c
 * 
 * Malloc wrapper
 */
#include "mem.h"
#ifdef LINUX
#include <execinfo.h>
#endif /* LINUX */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define UNUSED __attribute__ ((unused))

void *mem_alloc_location(size_t nbytes, const char *file, int line) {
	void *p;
#ifdef LINUX
	void *btbuf[500];
#endif /* LINUX */
		
	p = malloc(nbytes);
	if (p == NULL) {
#ifdef LINUX
		int n = backtrace(btbuf, 500);
		backtrace_symbols_fd(btbuf, n, fileno(stderr));
#endif /* LINUX */
		fprintf(stderr,
			"Memory allocation failure @ %s/line %d, %zd bytes\n",
			file, line, nbytes);
		abort();
	}
	return p;
}

void mem_free_location(void *ptr, UNUSED const char *file, UNUSED int line) {
	if (! ptr)
		return;
	free(ptr);
}

char *str_dupl_location(const char *s, const char *file, int line) {
	int n = strlen(s) + 1;
	char *p = (char *)mem_alloc_location(n, file, line);
	if (p)
		strcpy(p, s);
	return p;
}
