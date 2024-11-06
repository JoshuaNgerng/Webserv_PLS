#include "CheckFile.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cerr << "arg no prob\n";
        return (1);
    }
    int fd = open(av[1], O_RDONLY);
    int fd2 = open(av[2], O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        std::cerr << "open failed\n";
        return (1);
    }
    char    buffer[501];
    ssize_t r;
    std::string file;
    while ((r = read(fd, buffer, 500)) > 0) {
        buffer[r] = '\0';
        file.append(buffer, r);
    }
    std::cout << "r val: " << r << '\n';
    if (r < 0) {
        std::cerr << "error identified: " << strerror(errno) << "\n";
        return (1);
    }
    write(fd2, file.c_str(), file.length());
    return (0);
}