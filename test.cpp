#include "Server.hpp"
#include <csignal>

Server *ptr;

void testKill( int signum )
{
    swap_clear(*ptr); 
    exit(0);
}

int main(void)
{
    Server  server;
    ptr = &server;
    ptr->testAlloc();
    std::signal(SIGINT, &testKill);
    while(1);
    return (0);
}