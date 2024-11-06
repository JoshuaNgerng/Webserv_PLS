/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPipe.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:50:20 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/06 17:42:08 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiPipe.hpp"

CgiPipe::CgiPipe( void ) : File(), status(0), kill_child(false), child_id(-1) { }

CgiPipe::CgiPipe( const CgiPipe &src ) :
	File(src), status(src.status), kill_child(src.kill_child),
	child_id(src.child_id) { }

CgiPipe::~CgiPipe( void ) {
	if (child_id > 0 && kill_child) {
		kill(child_id, SIGKILL);
	}
	if (content_input_fd > 0) {
		close(content_input_fd);
	}
	if (content_output_fd > 0) {
		close(content_output_fd);
	}
}

CgiPipe&	CgiPipe::operator=( const CgiPipe &src ) {
	if (this == &src) {
		return (*this);
	}
	File::operator=(src);
	status = src.status;
	child_id = src.child_id;
	return (*this);
}

bool	CgiPipe::generateFds( void ) {
	int	pipefd[4];
	if (pipe(pipefd) < 0) {
		return (false);
	}
	content_input_fd = pipefd[0];
	if (pipe(&pipefd[2]) < 0) {
		close(pipefd[1]);
		return (false);
	}
	content_output_fd = pipefd[3];
	child_id = fork();
	if (child_id < 0) {
		return (false);
	}
	if (child_id > 0) {
		return (true);
	}
	exit(0);
	return (true);
}

bool	CgiPipe::checkStatus( void ) {
	int	check = waitpid(child_id, &status, WNOHANG);
	if (check == 0) {
		// timer ??
		return (true);
	}
	if (WIFEXITED(status) || WIFSIGNALED(status)) {
		std::cout << "Child process finished." << std::endl;
		return (false);
	}
	// other checks may need an int to get status
	// or other checks on status
	return (false);
}

std::ostream&	CgiPipe::print( std::ostream &o ) const {
	o << "CgiPipe id: " << child_id << ", status: " << status << "\n";
	return (o);
}
