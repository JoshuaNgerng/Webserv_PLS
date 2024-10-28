#include <iostream>
#include <ctime>
#include <unistd.h>

int main(void)
{
    time_t  t1, t2;
    time(&t1);
    // sleep(1);
    usleep(200000);
    time(&t2);
    std::cout << difftime(t2, t1) << '\n';
}