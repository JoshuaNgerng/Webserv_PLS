#include "Parse.hpp"

int	main(void) {
	Parse	parse("test.conf");
	try {
		parse.parseConfigFile();
		parse.checkParsing();
	}
	catch ( const std::exception &e ) {
		std::cerr << e.what() << '\n';
	}
	return (0);
}