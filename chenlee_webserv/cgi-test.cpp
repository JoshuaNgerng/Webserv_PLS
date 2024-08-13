#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>



int main()
{
    std::vector<const char *const> env;
    std::vector<const char *const> arg;

    std::string body ="Hello world";
    std::string uri = "cgi-bin/test.py";

    env.push_back("CONTENT_LENGTH=11");
    env.push_back("PATH_INFO=/cgi-bin/test.py");
    env.push_back("REQUEST_METHOD=GET");
    
    arg.push_back("/usr/bin/python3");
    arg.push_back(uri.c_str());
    arg.push_back(NULL);


    execve(arg[0], (char *const *)arg.data(), (char *const *)env.data());
    perror("execlp failed");
    exit(EXIT_FAILURE);
}