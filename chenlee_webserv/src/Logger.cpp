#include "Logger.hpp"



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Logger::Logger(std::string context)
{
    this->_context = context;
}

Logger::Logger( const Logger & src ): _context(src._context)
{
    std::cout << "this rand? " << this->_context << std::endl;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Logger::~Logger() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Logger &	Logger::operator=( Logger const & rhs )
{
	if ( this != &rhs )
	{
        this->_context = rhs._context;
	}
	return *this;
}

std::ostream &	operator<<( std::ostream & o, Logger const & i )
{
	(void)i;
	// o << "i am still here?";
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Logger::output(std::string color, std::string str) const
{
    time_t curr_time = time(NULL);
    char buffer[9];

    strftime(buffer, sizeof(buffer), "%X", localtime(&curr_time));

    // std::cout << buffer << std::endl;
    // std::cout << this->_context << std::endl;
    // std::cout << str << std::endl;

    std::cerr
        << color << buffer << "  " << this->_context << ": " << str << RESET << std::endl;
}

void Logger::log(std::string str) const
{
    output("", str);
}

void Logger::info(std::string str) const
{
    output(BLUE, str);
}

void Logger::warning(std::string str) const
{
    output(YELLOW, str);
}

void Logger::error(std::string str) const
{
    output(RED, str);
}
