/*
convert ipv4 string to sockaddr_in
then back into string to printf it
*/

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

int	main(void) {
	char				s[] = "127.0.0.1:8080";
	struct sockaddr_in	addr = {0}; 
	char				*ptr = strchr(s, ':');
	addr.sin_family = AF_INET;
	*ptr = '\0';
	inet_pton(addr.sin_family, s, &addr.sin_addr);
	addr.sin_port = htons(atoi(ptr + 1));
	char				buffer[INET_ADDRSTRLEN];
	int					port_no = ntohs(addr.sin_port);
	printf("test return %s\n", inet_ntop(addr.sin_family, &addr.sin_addr, buffer, INET_ADDRSTRLEN));
	printf("%s:%d\n", buffer, port_no);
}