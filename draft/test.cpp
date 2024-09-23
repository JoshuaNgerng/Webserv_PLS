#include <iostream>
#include <string>

int	main(void){
	std::string	str("12345667");
	std::string test = str.substr(2, std::string::npos);
	std::cout << test << '\n';
	return (0);
}