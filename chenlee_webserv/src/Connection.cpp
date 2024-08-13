#include "Connection.hpp"

int Connection::_connectionCount = 0;

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

Connection::Connection(int fd, ServerBlock *serverBlock) : fd(fd), _request(NULL), _response(NULL), _serverBlock(serverBlock), _logger(Logger("Connection"))
{
	this->_logger.log("Connection created at fd " + utl::toString(fd));
	_buffer.clear();
}

Connection::Connection(const Connection &src) : _logger(Logger("Connection"))
{
	_serverBlock = src._serverBlock;
	_request = src._request;
	_response = src._response;
	_buffer = src._buffer;
	fd = src.fd;
}

Connection::~Connection()
{
	delete _request;
	delete _response;
	this->_logger.log("Connection closed at fd " + utl::toString(fd));
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Connection &Connection::operator=(const Connection &rhs)
{
	if (this != &rhs)
	{
		_logger = rhs._logger;
		_serverBlock = rhs._serverBlock;
		_request = rhs._request;
		_response = rhs._response;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &o, Connection const &i)
{
	(void)i;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool Connection::readData()
{
	Router *router = &_serverBlock->getRouter();
	char buf[BUFFER_SIZE + 1]; // buffer for client data
	memset(buf, 0, BUFFER_SIZE + 1);
	try
	{

		ssize_t bytes_read = recv(fd, buf, BUFFER_SIZE, 0);
		size_t needle;
		if (bytes_read <= 0)
		{
			_logger.log(bytes_read == 0 ? "Connection closed by client" : "Read error");
			return false;
		}

		// append received data to buffer
		_buffer += std::string(buf, bytes_read);

		if (_request != NULL) // on going request
		{
			int res = _request->processBody(_buffer);
			if (res == 1) // body completed
			{
				this->_logger.info("handle with body");
				router->routeRequest(*_request, *_response);
			}
			else if (res != -1) // body not completed
				_buffer.clear();
		}
		else
		{
			needle = _buffer.find("\r\n\r\n");
			if (needle != std::string::npos) // found end of header
			{
				try
				{
					std::cout << _buffer.substr(0, needle) << std::endl;
					_request = new Request(_buffer.substr(0, needle + 4));
					_response = new Response(*this->_serverBlock);

					// check if hostname is recognized
					std::string requestHost = _request->getHeader("Host");
					std::string serverHostName = _serverBlock->getHostname();
					if (!serverHostName.empty() && requestHost.find(serverHostName) == std::string::npos && requestHost.find("localhost") == std::string::npos)
					{
						_logger.log("Hostname not recognized: " + requestHost);
						_response->errorResponse(404, "Hostname not recognized");
						return true;
					}

					int res = _request->checkIfHandleWithoutBody();
					if (res == 1)
					{
						this->_logger.log("handle without body");
						router->routeRequest(*_request, *_response);
					}
					else if (res == -1)
					{
						_response->errorResponse(404, "Invalid body");
					}
					else if (res == 0)
					{
						_buffer = _buffer.substr(needle + 4);
						if (_request->processBody(_buffer))
						{
							this->_logger.log("handle with body");
							router->routeRequest(*_request, *_response);
						}
					}
				}
				catch (const std::exception &e)
				{
					_logger.error(std::string(e.what()));
					_buffer.clear();
					return false;
				}
				_buffer.clear();
			}
		}
		return true;
	}

	catch (const std::exception &e)
	{
		_logger.error(std::string(e.what()));
		_buffer.clear();
		return false;
	}
	return true;
}

bool Connection::sendData(void)
{
	try
	{

		const std::string resString = _response->toString();
		if (resString.empty())
		{
			return false;
		}
		ssize_t bytes_sent = send(fd, resString.c_str(), resString.length(), 0);
		if (bytes_sent == -1)
		{
			perror("send");
			return false;
		}
		if ((unsigned long)bytes_sent < resString.length())
		{
			this->_logger.log("didnt send finish");
			_response->truncateResponse(bytes_sent);
			return false;
		}
		_logger.log("sent data");
		return true;
	}
	catch (const std::exception &e)
	{
		_logger.error(std::string(e.what()));
		return false;
	}
}

bool Connection::hasResponse()
{
	return _response != NULL && _response->getReady();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
