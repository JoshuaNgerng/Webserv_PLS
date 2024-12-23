/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cheader.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:28:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/02 03:51:46 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHEADER_H
# define CHEADER_H

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
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>


# ifndef UCHAR_MAX
#  define UCHAR_MAX 255
# endif

# ifndef UINT_MAX
#  define UINT_MAX 4294967295
# endif

# ifndef ULONG_MAX
#  define ULONG_MAX 18446744073709551615
# endif

# ifndef NULL
#  define NULL (0)
# endif

# ifndef size_t
typedef unsigned long size_t;
# endif

# ifndef ssize_t
typedef signed long ssize_t;
# endif

/**
 * fd (int) -> file descriptor (or socket)
 * events (short) -> events to monitor (requested events)
 * revents (short) -> events that actually occured (returned events)
 * 
 * Common event macros
 * POLLIN	-> fd is ready for reading without blocking
 * POLLOUT	-> fd is ready without blocking
 * POLLERR	-> error occured on fd
 * POLLHUP	-> hang up occured on fd (ex client close connection)
 * POLLNVAL	-> invalid fd (ex opened failed)
 * POLLPRI	-> urgent data to read (ex out-of-band data on TCP socket)
*/
typedef struct pollfd pollfd_t;

/**
 * int			ai_flags;
 * int			ai_family;
 * int			ai_socktype;
 * int			ai_protocol;
 * socklen_t	ai_addrlen;
 * struct sockaddr	*ai_addr;
 * char				*ai_canonname;
 * struct addrinfo	*ai_next;
 */
typedef struct addrinfo addrinfo_t;

/**
 * 
 */
typedef struct sockaddr_storage sockaddr_storage_t;

/**
 * sin_family domain name for ipv4 is AF_INET
 * sin_port port info example htons(port)
 * sin_addr.s_addr ip add info
 * sin_len exist on imac but not linux
 * sin_zero set to zero, padding
 */
typedef struct sockaddr_in sockaddr_in_t;

/**
 * sa_family (sa_family_t/ uint8_t)
 * sa_data (char[14])
 * sa_len (uint8_t)
*/
typedef struct sockaddr sockaddr_t;

#endif
