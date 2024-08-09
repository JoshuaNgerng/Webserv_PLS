/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cheader.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:28:01 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/09 12:14:06 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHEADER_H
# define CHEADER_H

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

# ifndef UCHAR_MAX
#  define UCHAR_MAX 255
# endif

# ifndef NULL
#  define NULL (0)
# endif

# ifndef NULL
#  define N a
# endif

typedef struct pollfd pollfd_t;
typedef struct sockaddr_in sockaddr_t;

#endif