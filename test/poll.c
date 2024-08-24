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
# include <stdio.h>

int	main(void) {
	struct sockaddr_in addr = {.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY, .sin_port = htons(8080)};
	socklen_t	len = sizeof(addr);
	int fd = socket(addr.sin_family, SOCK_STREAM, 0);
	if (fd < 0)
		return (1);
	if (bind(fd, (struct sockaddr *)(&addr), len) < 0)
		return (1);
	if (listen(fd, 10) < 0)
		return (1);
	struct pollfd pfd = {.fd = fd, .events = POLLIN};
	int	i = -1;
	while (++ i < 20)
	{
		poll(&pfd, 1, 10000);
		if (pfd.revents == 0)
			printf("Empty POll revent\n");
		if (pfd.revents & POLLIN)
			printf("POLLIN true\n");
		if (pfd.events & POLLOUT)
			printf("POLLOUT true\n");
	}
	close(fd);
	return (0);
}

// int	main(void) {
// 	int fd = open("test.txt", O_RDONLY | O_WRONLY);
// 	if (fd < 0)
// 		return (1);
// 	struct pollfd pfd = {.fd = fd, .events = POLLIN};
// 	int	i = -1;
// 	while (++ i < 20)
// 	{
// 		poll(&pfd, 1, 100);
// 		if (pfd.revents & POLLIN)
// 			printf("POLLIN true\n");
// 		if (pfd.events & POLLOUT)
// 			printf("POLLOUT true\n");
// 		if (i == 5)
// 			pfd.events = POLLOUT;
// 	}
// 	close(fd);
// 	return (0);
// }

