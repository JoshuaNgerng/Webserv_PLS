#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    struct addrinfo hints, *res;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // Use either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME | AI_PASSIVE;  // Request canonical name

    // Get address information for www.example.com
    if ((status = getaddrinfo("::255", NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    // Print the canonical name
    printf("Canonical name: %s\n", res->ai_canonname);

    // Convert the IP address to a string and print it
    void *addr;
    if (res->ai_family == AF_INET) {  // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        addr = &(ipv4->sin_addr);
    } else {  // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)res->ai_addr;
        addr = &(ipv6->sin6_addr);
    }
    inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
    printf("IP address: %s\n", ipstr);

    freeaddrinfo(res);
    return 0;
}
