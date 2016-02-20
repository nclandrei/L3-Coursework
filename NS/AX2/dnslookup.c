#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFLEN 1500

int main(int argc, char *argv[]) {
    struct addrinfo hints, *ai, *ai0;    
    char ip_address[BUFLEN];
    struct sockaddr_in *h;
    int i;

    if (argc < 2) {
        printf("Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((i = getaddrinfo(argv[1], NULL, &hints, &ai0)) != 0) {
        printf("Unable to look up IP address: %s", gai_strerror(i));
    }

    for (ai = ai0; ai != NULL; ai = ai->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *) ai->ai_addr;
        void *addr = &(ipv4->sin_addr);
        if(ai->ai_family != AF_INET) {
            continue;
        }   
        inet_ntop(AF_INET, addr, ip_address, BUFLEN);
        printf("%s\n", ip_address);
    }
    freeaddrinfo(ai0);
    return 0;
}
