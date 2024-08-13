#include "ServerBlock.hpp"

ServerBlock::ServerBlock()
{
	listen = 0;
	router = Router(this);

	char cwd[PATH_MAX];
#ifdef _WIN32
	if (GetCurrentDirectory(PATH_MAX, cwd) != 0)
	{
		_projectDir = cwd;
	}
	else
	{
		printf("Failed to get project directory: %lu\n", GetLastError());
	}
#else
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		_projectDir = cwd;
	}
	else
	{
		perror("Failed to get project directory");
	}
#endif

	// Append "/" to the end of _projectDir if it's not already there
	if (!_projectDir.empty())
	{
#ifdef _WIN32
		if (_projectDir.back() != '\\')
			_projectDir += "\\";
#else
		if (_projectDir.back() != '/')
			_projectDir += "/";
#endif
	}
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void ServerBlock::display() const
{
	router.display();
}

string ServerBlock::getHostname() const
{
	return this->hostname;
}

string ServerBlock::handleRequest(string request)
{
	std::cout << "Handling request on port " << listen << std::endl;
	std::string copy(request);
	if (copy.find("\r\n\r\n") != std::string::npos)
	{
		std::cout << "Request complete" << std::endl;
		std::cout << GREEN << request << RESET << std::endl;
		return "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	}
	return "";
}

void ServerBlock::addErrorPage(int code, std::string path)
{
	_errorPages[code] = path;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


void ServerBlock::setBodyLimit(int bodyLimit)
{
	this->bodyLimit = bodyLimit;
}

void ServerBlock::setListen(int listen)
{
	this->listen = listen;
}

int ServerBlock::getBodyLimit() const
{
	return bodyLimit;
}

Router &ServerBlock::getRouter()
{
	return router;
}

int ServerBlock::getListen() const
{
	return listen;
}

IntStringMap& ServerBlock::getErrorPages()
{
	return _errorPages;
}

std::string ServerBlock::getRoot() const
{
	return _root;
}
void ServerBlock::setRoot(std::string root)
{
	_root = root;
}

std::string ServerBlock::getProjectDir() const
{
	return _projectDir;
}
