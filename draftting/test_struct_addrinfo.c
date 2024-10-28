#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p, **tail;
    int status;
    char ipstr[INET6_ADDRSTRLEN];
    hints = (struct addrinfo){0};

    if (argc != 2) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }

    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    tail = &res;
    if ((status = getaddrinfo("www.const.com", "443", &hints, tail)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    p = *tail;
    while (p->ai_next) {
        p = p->ai_next;
    }
    tail = &(p->ai_next);
    if ((status = getaddrinfo(argv[1], "80", &hints, tail)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }


    for(p = res;p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;
        int  port;

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
            port = ntohs(ipv4->sin_port);
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
            port = ntohs(ipv6->sin6_port);
        }

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s -> %s: %s:%d\n", p->ai_canonname, ipver, ipstr, port);
    }

    freeaddrinfo(res); // free the linked list

    return 0;
}
