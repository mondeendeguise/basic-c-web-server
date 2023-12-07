#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int socketfd;
    int errnum;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    if(argc < 3) {
        printf("Usage: %s <URL> <PORT> <MSG>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;            // Any protocol
    hints.ai_flags = 0;

    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    // result is set here
    errnum = getaddrinfo(argv[1], argv[2], &hints, &result);
    if(errnum != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errnum));
        exit(EXIT_FAILURE);
    }

    for(rp = result; rp != NULL; rp = rp->ai_next) {
        fprintf(stderr, "Processing socket: %d:%d\n", rp->ai_family, rp->ai_protocol);
        socketfd = socket(rp->ai_family, rp->ai_socktype, 0);
        if(socketfd == -1) {
            fprintf(stderr, "socket: %s\n", strerror(errno));
            continue;
        }

        fprintf(stderr, "Connecting to socket...\n");
        if(connect(socketfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            fprintf(stderr, "Eurethra!\n");
            break;  // SUCCESS
        }

        if(close(socketfd) != 0) {
            fprintf(stderr, "close: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // Connection has been made, we no longer need the original result
    freeaddrinfo(result);

    if(rp == NULL) {  // No address succeeded
        fprintf(stderr, "Failed to establish connection\n");
        exit(EXIT_FAILURE);
    }

    for(size_t i = 3; i < argc; i++) {

    }

    close(socketfd);
    return 0;
}
