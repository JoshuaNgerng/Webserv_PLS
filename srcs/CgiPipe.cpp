/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPipe.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:50:20 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/07 22:24:30 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiPipe.hpp"
#include "Server.hpp"
#include "Client.hpp"

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

bool	CgiPipe::setupPipes( int pipefd[4] ) {
	if (pipe(pipefd) < 0) {
		return (false);
	}
	content_input_fd = pipefd[0];
	if (pipe(&pipefd[2]) < 0) {
		close(pipefd[1]);
		return (false);
	}
	content_output_fd = pipefd[3];
	return (true);
}

bool	CgiPipe::getBinary( void ) {
	std::ifstream	script(content_id.c_str());
	if (!script.is_open()) {
		return (false);
	}
	std::string	buffer;
	if (std::getline(script, buffer)) {
		size_t pos = buffer.find("#!");
		if (pos != std::string::npos) {
			pos += 2;
			size_t end = pos;
			for (size_t i = pos; i < buffer.length(); i ++) {
				if (std::isspace(static_cast<int>(buffer[i]))) {
					end = i;
					break;
				}
			}
			bin_path = buffer.substr(pos, end - pos);
			return (true);
		}
	} else {
		return (false);
	}
	bin_path = content_id;
	return (true);
}

void	CgiPipe::getClientInfoReq( const HttpRequest &req ) {
	static int	diff = 'a' - 'A';
	for (size_t i = 0; Http::fields[i]; i ++) {
		std::string	buffer("HTTP_");
		std::string	buffer_field(Http::fields[i]);
		for (size_t j = 0; j < buffer_field.length(); j ++) {
			if (std::islower(static_cast<unsigned char>(buffer_field[j]))) {
				buffer_field[j] =- diff;
			}
		}
		buffer += buffer_field;
		buffer += "=";
		buffer += req.getField(Http::fields[i]);
		req_info.push_back(buffer);
	}
}

void	CgiPipe::getClientInfoHelper( const char *key, const std::string &val ) {
	std::string	buffer(key);
	buffer += "=";
	buffer += val;
	req_info.push_back(buffer);
}

void	CgiPipe::getClientInfo( const Client &client ) {
	const HttpRequest&	req = client.getCurrentHttpRequest();
	getClientInfoReq(req);
	getClientInfoHelper("REQUEST_METHOD", client.getReqMethod());
	getClientInfoHelper("CONTENT_TYPE", client.getContentType());
	getClientInfoHelper("CONTENT_LENGTH", to_String(client.getContentLength()));
	getClientInfoHelper("REMOTE_ADDR", client.getAddr());
	getClientInfoHelper("SERVER_NAME", req.getField("host"));
	getClientInfoHelper("SCRIPT_NAME", client.getCurrentPath());
	getClientInfoHelper("QUERY_STRING", client.getCurrentQuery());
	getClientInfoHelper("REQUEST_URI", client.getCurrentUri());
	getClientInfoHelper("DOCUMENT_URI", client.getCurrentUri());
}

void	CgiPipe::addEnv( const Client &client, char *const *env_ ) {
	for (size_t i = 0; env_[i]; i ++) {
		env.push_back(env_[i]);
	}
	getClientInfo(client);
	for (size_t i = 0; req_info.size(); i ++) {
		env.push_back(req_info[i].c_str());
	}
	env.push_back(NULL);
}

int	CgiPipe::execvChild( int pipefd[4] ) {
	int	check = 0;
	if (dup2(STDIN_FILENO, pipefd[2]) < 0) {
		check = 1;
	}
	if (dup2(STDOUT_FILENO, pipefd[1]) < 0) {
		check = 1;
	}
	for (int i = 0; i < 4; i ++) {
		close(pipefd[i]);
	}
	if (check) {
		return (500);
	}
	if (!bin_path.length()) {
		if (!getBinary()) {
			return (404);
		}
	}
	std::vector<const char *>buffer_exec;
	buffer_exec.push_back(content_id.c_str());
	buffer_exec.push_back(NULL);
	execve(
		bin_path.c_str(),
		const_cast<char**>(&buffer_exec[0]),
		const_cast<char**>(&env[0])
	);
	return (500);
}

bool	CgiPipe::generateFds( void ) {
	int	pipefd[4];
	if (!setupPipes(pipefd)) {
		return (false);
	}
	child_id = fork();
	if (!child_id) {
		int check = execvChild(pipefd);
		throw ChildProcess(check);
	}
	close(pipefd[1]);
	close(pipefd[2]);
	if (child_id < 0) {
		return (false);
	}
	if (child_id > 0) {
		return (true);
	}
	return (true);
}

bool	CgiPipe::checkStatus( void ) {
	if (child_id < 0) {
		return (false);
	}
	int	check = waitpid(child_id, &status, WNOHANG);
	if (check == 0) {
		// timer ??
		kill_child = false;
		return (true);
	}
	if (WIFEXITED(status) || WIFSIGNALED(status)) {
		std::cout << "Child process finished\n";
		return (false);
	}
	// other checks may need an int to get status
	// or other checks on status
	return (false);
}

CgiPipe::ChildProcess::ChildProcess( int status ) : status_code(status) { }

CgiPipe::ChildProcess::~ChildProcess( void ) { }

const char*	CgiPipe::ChildProcess::what() const throw() { return ("ChildProcess Cant Execute"); }

int	CgiPipe::ChildProcess::getExitCode( void ) const { return (status_code); }

std::ostream&	CgiPipe::print( std::ostream &o ) const {
	o << "CgiPipe id: " << child_id << ", status: " << status << "\n";
	return (o);
}
