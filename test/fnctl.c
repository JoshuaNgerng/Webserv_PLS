# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <poll.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>

void    print_bits(int bits, int size) {
    {
    for(int bit=0;bit<size; bit++)
        {
         printf("%i ", bits & 0x01);
         bits = bits >> 1;
     }
    }
    printf("\n");
}

int main(void) {
    char				s[] = "127.0.0.1:11";
	struct sockaddr_in	addr = {0}; 
	char				*ptr = strchr(s, ':');
	addr.sin_family = AF_INET;
	*ptr = '\0';
	inet_pton(addr.sin_family, s, &addr.sin_addr);
	addr.sin_port = htons(atoi(ptr + 1));
    // int fd = open("ip_test.c", O_RDONLY);
    int fd = socket(addr.sin_family, SOCK_STREAM, 0);
    int flagss = -1;
    if (fd < 0)
        return (1);
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        close(fd);
		return (1); // cant set nonblock flag
	}
    flagss = fcntl(fd, F_GETFL);
	if (flagss < 0) {
        close(1);
		return (1); // cant get flags
	}
   print_bits(flagss, sizeof(flagss) * 4);
	// flagss |= O_NONBLOCK;
//    print_bits(flagss, sizeof(flagss) * 4); 
    close(fd);
    return (0);
}
