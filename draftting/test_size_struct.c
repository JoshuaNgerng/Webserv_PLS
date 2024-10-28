#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(void)
{
    printf("%lu %lu %lu\n", sizeof(struct sockaddr), sizeof(struct sockaddr_in), sizeof(struct sockaddr_in6));
    return (0);
}