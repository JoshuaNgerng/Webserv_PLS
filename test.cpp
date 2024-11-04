#include "CheckFile.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cout << "no file\n";
        return (0);
    }
    CheckFile   check(av[1]);
    check.checking();
    std::cout << "size " << check.getFilesize() << '\n';
    return (0);
}