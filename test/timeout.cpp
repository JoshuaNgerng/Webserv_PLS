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
# include <ctime>
# include <iostream>

int	main(void) {
	time_t	t1, t2;
	struct pollfd pfd = {.fd = 0, .events = POLLIN, .revents = 0};
	int	i = -1;
	time(&t1);
	while (++ i < 20)
	{
		poll(&pfd, 1, 10000 /*comment*/);
		if (pfd.revents == 0) {
			std::cout << "Empty POll revent\n"; //huh
			time(&t2);
			std::cout << difftime(t2, t1) << '\n';
			continue ;
		}
		if (pfd.revents & POLLIN)
		{
			char	s[100];
			std::cout << "try read\n";
			ssize_t	r = read(0, s, 100);
			if (r < 0) {
				std::cout << "read return neg\n";
				continue ;
			}
			s[r] = '\0';
			time(&t1);
			std::cout << "finish read " << s;
		}
		else
			std::cout << "POLL REVENT ERR\n";	
	}
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
