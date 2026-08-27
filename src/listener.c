#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "4950"

#define MAXBUFLEN 100

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() {
    int sockfd;
    int rv, num_bytes;
    struct sockaddr_storage their_addr;
    struct addrinfo hints, *servinfo, *p;

    char buf[MAXBUFLEN];
    char s[INET6_ADDRSTRLEN];

    socklen_t addr_len;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) == -1) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    if ((num_bytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
        (struct sockaddr*)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);        
    }

    inet_ntop(their_addr.ss_family, 
        get_in_addr((struct sockaddr*)&their_addr),
        s, sizeof s);
    printf("listener: got packet from %s\n", s);
    printf("listener: packet is %d bytes long\n", num_bytes);

    
    buf[num_bytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);

    close(sockfd);

    return 0;
}