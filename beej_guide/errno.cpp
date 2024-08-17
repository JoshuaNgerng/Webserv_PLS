#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(){
    FILE *file = fopen("nonexistent.txt", "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
    }
}
 //LOL just like that