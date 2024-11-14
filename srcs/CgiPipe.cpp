/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPipe.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:50:20 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/15 04:38:02 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiPipe.hpp"
#include "Client.hpp"
#include "Server.hpp"

CgiPipe::CgiPipe( void ) :
	File(), status(0), kill_child(false), child_id(-1),
	directory(""), bin_path(""), req_info(), env(), ptr(), child_done(false) { }

CgiPipe::CgiPipe( const std::string &bin, Client &client ) :
	File(), status(0), kill_child(false), child_id(-1),
	directory(""), bin_path(bin), req_info(),
	env(), ptr(&client), child_done(false) { }

CgiPipe::CgiPipe( const CgiPipe &src ) :
	File(src), status(src.status), kill_child(src.kill_child),
	child_id(src.child_id), directory(src.directory), bin_path(src.bin_path),
	req_info(src.req_info), env(src.env), ptr(src.ptr), child_done(src.child_done) { }

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
	kill_child = src.kill_child;
	child_id = src.child_id;
	directory = src.directory;
	bin_path = src.bin_path;
	req_info = src.req_info;
	env = src.env;
	ptr = src.ptr;
	child_done = src.child_done;
	return (*this);
}

bool	CgiPipe::setupPipes( void ) {
	if (pipe(pipefd) < 0) {
		return (false);
	}
	content_input_fd = pipefd[0];
	if (pipe(pipefd +2) < 0) {
		close(pipefd[1]);
		return (false);
	}
	content_output_fd = pipefd[3];
	return (true);
}

bool	CgiPipe::changeDir( void ) {
	size_t pos = content_id.find_last_of('/');
	if (pos == std::string::npos) {
		return (true);
	}
	directory = content_id.substr(0, pos);
	if (chdir(directory.c_str()) < 0) {
		return (false);
	}
	content_id.erase(0, pos + 1);
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
			size_t end = buffer.length();
			for (size_t i = pos; i < buffer.length(); i ++) {
				if (std::isspace(static_cast<unsigned char>(buffer[i]))) {
					end = i;
					break;
				}
			}
			if (pos == end) {
				bin_path = content_id;
				return (true);
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
	std::string	buffer;
	for (size_t i = 0; Http::fields[i]; i ++) {
		std::string	buffer_field(Http::fields[i]);
		for (size_t j = 0; j < buffer_field.length(); j ++) {
			if (std::islower(static_cast<unsigned char>(buffer_field[j]))) {
				buffer_field[j] -= diff;
			} else if (buffer_field[j] == '-') {
				buffer_field[j] = '_';
			}
		}
		buffer = "HTTP_";
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
	for (size_t i = 0; i < req_info.size(); i ++) {
		env.push_back(req_info[i].c_str());
	}
	env.push_back(NULL);
}

void	CgiPipe::addDir( const std::string &path ) { directory = path; }

void	CgiPipe::addBin( const std::string &content_name, const std::string &bin ) {
	content_id = content_name;
	bin_path = bin;
}

int	CgiPipe::execvChild( void ) {
	bool	check = false;
	if (!changeDir()) {
		return (500);
	}
	if (dup2(pipefd[2], STDIN_FILENO) < 0) {
		check = true;
	}
	if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
		check = true;
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
	buffer_exec.push_back(bin_path.c_str());
	if (bin_path != content_id) {
		buffer_exec.push_back(content_id.c_str());
	}
	buffer_exec.push_back(NULL);
	
	execve(
		bin_path.c_str(),
		const_cast<char**>(&buffer_exec[0]),
		const_cast<char**>(&env[0])
	);
	return (500);
}

bool	CgiPipe::generateFds( void ) {
	if (!setupPipes()) {
		return (false);
	}
	counter = 2;
	addEnv(*ptr, Server::env);
	child_id = fork();
	if (child_id == 0) {
		throw ChildProcess(*this);
	}
	close(pipefd[1]);
	close(pipefd[2]);
	if (child_id < 0) {
		return (false);
	}
	kill_child = true;
	return (true);
}

bool	CgiPipe::checkStatus( void ) {
	if (child_id < 0) {
		return (false);
	}
	if (child_done) {
		return ((status) ? false : true);
	}
	int	check = waitpid(child_id, &status, WNOHANG);
	if (check == 0) {
		kill_child = true;
		return (true);
	} else {
		child_done = true;
	}
	kill_child = false;
	if (status) {
		status = WIFEXITED(status);
		if (status != 404) {
			status = 500;
		}
		ptr->addContent(status);
		return (false);
	}
	return (true);
}

CgiPipe::ChildProcess::ChildProcess( CgiPipe &ref ) : ptr(&ref) { }

CgiPipe::ChildProcess::~ChildProcess( void ) throw() { delete ptr; }

const char*	CgiPipe::ChildProcess::what() const throw() {
	return ("ChildProcess Execute");
}

int	CgiPipe::ChildProcess::execveCgiPipe( void ) { return(ptr->execvChild()); }

std::ostream&	CgiPipe::print( std::ostream &o ) const {
	o << "CgiPipe id: " << child_id << ", status: " << status;
	if (!child_id) {
		o << ", this is Child process";
	}
	o << '\n';
	if (kill_child) {
		o << "desctructor kill child";
	} else {
		o << "kill child off";
	}
	return (o);
}
