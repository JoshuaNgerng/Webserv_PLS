/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshua <joshua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:56:15 by jngerng           #+#    #+#             */
/*   Updated: 2024/09/21 19:02:29 by joshua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CheckFile.hpp"

CheckFile::CheckFile( void ) { }

CheckFile::CheckFile( const char *filename_ ) : filename(filename_) { }

CheckFile::~CheckFile( void ) { }

void	CheckFile::checking( void ) {
	checking(F_OK | W_OK | R_OK | X_OK);
}

void	CheckFile::checking( int check_flags ) {
	struct stat stat_info;
	acessiblity = access(filename.c_str(), check_flags);
	type = error;
	if (stat(filename.c_str(), &stat_info) < 0)
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
}

uint8_t	CheckFile::getType( void ) const { return (type); }

uint8_t	CheckFile::getAccessbility( void ) const { return (acessiblity); }

size_t	CheckFile::getFilesize( void ) const { return (filesize); }

bool	CheckFile::fileToStringStream( std::stringstream &dst, std::ifstream &file ) {
	if (!(file.is_open()))
		return (false);
	dst << file.rdbuf();
	return (true);
}

bool	CheckFile::getFileContent( std::string &dst ) const {
	std::ifstream		config_file(filename.c_str());
	std::stringstream	fileStream;
	if (!fileToStringStream(fileStream, config_file))
		return (false);
	fileStream << config_file.rdbuf();
	dst = fileStream.str();
	return (true);
}

bool	CheckFile::getFileContent( std::stringstream &dst ) const {
	std::ifstream		config_file(filename.c_str());
	std::stringstream	fileStream;
	if (!fileToStringStream(dst, config_file))
		return (false);
	return (true);
}
