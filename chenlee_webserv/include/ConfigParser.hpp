/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chenlee <chenlee@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:45:30 by jatan             #+#    #+#             */
/*   Updated: 2024/08/10 21:06:47 by chenlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>



// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <netdb.h>	// struct addrinfo, gai_strerror
// # include <unistd.h> // execve, dup, dup2, pipe, fork

# include "Logger.hpp"
# include "ServerBlock.hpp"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::vector;

class ConfigParser {
public:
    void createServerBlocksFromConf(const std::string& filename, std::vector<RequestHandler*>& requestHandlers, std::vector<ServerBlock*>& serverBlocks);
    void displayConfig(ServerBlock& server) const;

private:
    void parseServerConfig(std::stringstream& ss, std::string& key, ServerBlock& server);
    void parseLocationConfig(std::stringstream& ss, std::string& key, RouteDetails& routeDetails);
    static void trim(std::string& str);
};

#endif /* ****************************************************** CONFIG_PARSER_HPP */
