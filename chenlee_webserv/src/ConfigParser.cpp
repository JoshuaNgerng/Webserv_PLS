#include "ConfigParser.hpp"

void ConfigParser::createServerBlocksFromConf(const std::string &filename, std::vector<RequestHandler *> &requestHandlers, std::vector<ServerBlock *> &serverBlocks)
{
	std::ifstream configFile(filename.c_str());
	std::string line;
	ServerBlock *server = NULL;
	bool inServerBlock = false, inLocationBlock = false;
	RouteDetails *routeDetails;
	std::string currentRoute;

	if (!configFile.is_open())
	{
		std::cerr << "Failed to open config file." << std::endl;
		exit(1);
	}

	while (getline(configFile, line))
	{
		trim(line); // remove leading and trailing whitespaces
		if (line.empty() || line[0] == '#')
			continue; // skip empty lines and comments

		std::stringstream ss(line);
		std::string key, value, end;
		ss >> key;

		if (key == "server")
		{
			ss >> end;
			if (end == "{")
			{
				inServerBlock = true;
				// create new server block
				serverBlocks.push_back(new ServerBlock());
				server = serverBlocks.back();
				server->getRouter().assignHandlers(requestHandlers);
				continue;
			}
			else
			{
				std::cerr << "Key server is reserved for server block\nie: server {...}" << std::endl;
				return;
			}
		}
		else if (key == "location")
		{
			ss >> value;
			ss >> end;
			if (value.empty() || end != "{")
			{
				std::cerr << "Invalid route block\nie: route /pathname {...}" << std::endl;
				exit(1);
			}
			else if (inServerBlock == false)
			{
				std::cerr << "route should be inside a server {}" << std::endl;
				exit(1);
			}
			currentRoute = value;
			routeDetails = new RouteDetails(value);
			inLocationBlock = true;
			continue;
		}
		else if (key == "}")
		{
			if (inLocationBlock)
			{
				server->getRouter().addRoute(currentRoute, routeDetails);
				currentRoute = "";
				routeDetails = NULL;
				inLocationBlock = false;
			}
			else if (inServerBlock)
			{
				server = NULL;
				inServerBlock = false;
			}
		}
		else if (inLocationBlock)
		{
			parseLocationConfig(ss, key, *routeDetails);
		}
		else if (inServerBlock)
		{
			parseServerConfig(ss, key, *server);
		}
	}
	configFile.close();
}

void ConfigParser::parseServerConfig(std::stringstream &ss, std::string &key, ServerBlock &server)
{
	std::string value;
	if (key == "listen")
	{
		int port;
		ss >> port;
		server.setListen(port);
	}
	else if (key == "body_limit")
	{
		int bodyLimit;
		ss >> bodyLimit;
		server.setBodyLimit(bodyLimit);
	}
	else if (key == "error_page")
	{
		int statusCode;
		std::string path;
		ss >> statusCode >> path;
		server.addErrorPage(statusCode, path);
	}
	else if (key == "root")
	{
		std::string root;
		ss >> root;
		server.setRoot(root);
	}

	else if (key == "hostname")
	{
		ss >> server.hostname;
	}
}

void ConfigParser::parseLocationConfig(std::stringstream &ss, std::string &key, RouteDetails &routeDetails)
{
	std::string value;
	if (key == "root")
	{
		ss >> routeDetails.root;
	}
	else if (key == "index")
	{
		ss >> routeDetails.index;
	}
	else if (key == "autoindex")
	{
		ss >> value;
		routeDetails.autoindex = (value == "on");
	}
	else if (key == "allowed_methods")
	{
		while (ss >> value)
		{
			if (value == "GET")
				routeDetails.allowedMethods |= GET; // add GET to allowedMethods
			else if (value == "POST")
				routeDetails.allowedMethods |= POST; // add POST to allowedMethods
			else if (value == "DELETE")
				routeDetails.allowedMethods |= DELETE; // add DELETE to allowedMethods
			else
			{
				std::cerr << "Invalid method: " << value << std::endl;
				return;
			}
		}
	}
	else if (key == "cgi_pass")
	{
		ss >> routeDetails.cgiPass;
	}
	else if (key == "redirect")
	{
		ss >> routeDetails.redirection;
	}
}

void ConfigParser::displayConfig(ServerBlock &server) const
{
	server.display();
}

void ConfigParser::trim(std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	if (first == std::string::npos || last == std::string::npos)
	{
		str = "";
	}
	else
	{
		str = str.substr(first, (last - first + 1));
	}
}
