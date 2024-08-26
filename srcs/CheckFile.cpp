/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:56:15 by jngerng           #+#    #+#             */
/*   Updated: 2024/08/26 10:11:44 by jngerng          ###   ########.fr       */
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
	if (stat_info.st_mode & S_IFREG)
		type = file;
	else if (stat_info.st_mode & S_IFDIR)
		type = folder;
	else
		type = unknown;
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

uint8_t	CheckFile::getFileContent( std::string &dst ) const {
	std::ifstream		config_file(filename.c_str());
	std::stringstream	fileStream;
	if (!fileToStringStream(fileStream, config_file))
		return (open_);
	fileStream << config_file.rdbuf();
	dst = fileStream.str();
	return (success);
}

uint8_t	CheckFile::getFileContent( std::stringstream &dst ) const {
	std::ifstream		config_file(filename.c_str());
	std::stringstream	fileStream;
	if (!fileToStringStream(dst, config_file))
		return (open_);
	return (success);
}
