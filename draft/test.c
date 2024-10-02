#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void) {
    const char *ptr = "srcs/";
    int a = access(ptr, F_OK | R_OK);
    printf("access %d\n", a);
    struct stat stat_info;
    if (stat(ptr, &stat_info) < 0) {
        return (1);
    }
    int t = stat_info.st_mode & S_IFMT;
    if (t == S_IFDIR)
        printf("is directory\n");
    else if (t == S_IFREG)
        printf("is file\n");
    return (0);
}