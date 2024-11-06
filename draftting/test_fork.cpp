# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/socketvar.h>
# include <sys/select.h>
# include <poll.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <netdb.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <list>
# include <string>
# include <iostream>

class Testing {
    public:
        Testing( void ) { 
            mem.push_back("123");
            mem.push_back("234");
            mem.push_back("ufeshhfliheslilihflifes");
            mem.push_back("O#Ho8HR#O*HO*R#HO*RHo8hf");
        }
        ~Testing( void ) { }
    private:
        std::list<std::string>  mem;
};

int main(void) {
    Testing test;
    pid_t   id = -1;
    id = fork();
    if (id == 0) {
        sleep(5);
        exit(0);
    }
    if (id < 0) {
        std::cerr << "cant fork\n";
        return (1);
    }
    int status = 0;
    waitpid(id, &status, 0);
    sleep(2);
    std::cout << "status of child: " << id << " is " << status << "\n";
    return (0);
}