#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

struct test {
    int     a;
    int     b;
    float   c;
    float   d;
    int     e;
};

int main(void) {
    static struct addrinfo addr = {AF_UNSPEC, SOCK_STREAM};
    struct test t = {11, -1};
    std::cout << t.a << ' ' << t.b << ' ' << t.c <<  ' ' << t.d << ' ' << t.e << '\n';
    return (0);
}