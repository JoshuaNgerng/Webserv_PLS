/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:56:15 by jngerng           #+#    #+#             */
/*   Updated: 2024/11/15 06:54:47 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CheckFile.hpp"

CheckFile::CheckFile( void ) { }

CheckFile::CheckFile( const char *filename_ ) : filename(filename_) { }

CheckFile::CheckFile( const std::string &filename_ ) : filename(filename_.c_str()) { }

CheckFile::~CheckFile( void ) { }

CheckFile::CheckFile(const CheckFile &other) {
	*this = other;
}

CheckFile& CheckFile::operator=(const CheckFile &other) {
	if (this != &other) {
		filename = other.filename;
		type = other.type;
		acessiblity = other.acessiblity;
		filesize = other.filesize;
		timeinfo = other.timeinfo;
	}
	return *this;
}

 bool	CheckFile::checkAccessbility( const char *path, int mask ) {
	if (access(path, mask) < 0) {
		return (false);
	}
	return (true);
 }

void	CheckFile::checking( void ) {
	checking(F_OK | W_OK | R_OK | X_OK);
}

void	CheckFile::checking( int check_flags ) {
	struct stat stat_info;
	acessiblity = access(filename, check_flags);
	type = error;
	if (stat(filename, &stat_info) < 0)
		return ;
	switch (stat_info.st_mode & S_IFMT) {
		case (S_IFBLK): 	type = block_device;	break;
		case (S_IFCHR): 	type = char_device;		break;
		case (S_IFDIR): 	type = directory;		break;
		case (S_IFIFO): 	type = fifo_pipe;		break;
		case (S_IFLNK): 	type = systemlink;		break;
		case (S_IFREG): 	type = file;			break;
		case (S_IFSOCK): 	type = sock;			break;
		default: 			type = unknown;			break;
	}
	filesize = stat_info.st_size;
	time_t	buffer;
	#ifdef __APPLE__
		buffer = stat_info.st_mtimespec.tv_sec;
	#else // can't use #elif
		buffer = stat_info.st_mtim.tv_sec;
	#endif
	timeinfo = std::localtime(&buffer);
}

uint8_t	CheckFile::getType( void ) const { return (type); }

int8_t	CheckFile::getAccessbility( void ) const { return (acessiblity); }

size_t	CheckFile::getFilesize( void ) const { return (filesize); }

const struct tm*	CheckFile::getTime( void ) const { return(timeinfo); }

const char*	CheckFile::fetchExtension( void ) const {
	size_t len = -1;
	while (filename[++len]);
	for (size_t i = len - 2; i != 0; i --) {
		if (filename[i] == '.') {
			return (&filename[i + 1]);
		}
	}
	return (NULL);
}

const char*	CheckFile::fetchExtension( const std::string &fname ) {
	static const char *empty = "";
	size_t pos = fname.find_last_of('.');
	if (pos == std::string::npos || pos == fname.length()) {
		return (empty);
	}
	return (fname.c_str() + pos + 1);
}

bool	CheckFile::fileToStringStream( std::stringstream &dst, std::ifstream &file ) {
	if (!(file.is_open()))
		return (false);
	dst << file.rdbuf();
	return (true);
}

bool	CheckFile::getFileContent( std::string &dst ) const {
	std::ifstream		config_file(filename);
	std::stringstream	fileStream;
	if (!fileToStringStream(fileStream, config_file))
		return (false);
	fileStream << config_file.rdbuf();
	dst = fileStream.str();
	return (true);
}

bool	CheckFile::getFileContent( std::stringstream &dst ) const {
	std::ifstream		config_file(filename);
	std::stringstream	fileStream;
	if (!fileToStringStream(dst, config_file))
		return (false);
	return (true);
}
