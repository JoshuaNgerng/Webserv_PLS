#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <vector>

class Parse {
	typedef std::vector<std::pair<std::string, std::string>>::iterator it;
	private:
		std::string											header_file;
		std::string											source_file;
		std::string											class_name;
		std::vector<std::pair<std::string, std::string>>	private_variables;
		std::vector<std::pair<std::string, std::string>>	private_variables_compound;
		bool	class_found;
		bool	private_found;

		bool	checkCompoundType( const std::string &token ) {
			static const char** stl = (char *[]){"vector", "map", "list", "deque", NULL};
			for (int i = 0; stl[i]; i ++) {
				if (token.find(stl[i], 0) != std::string::npos)
					return (true);
			}
			return (false);
		}

		bool	processTokens( const std::string &token ) {
			if (token == "class") {
				class_found = true;
				return (false);
			}
			if (token == "private:") {
				private_found = true;
				return (false);
			}
			if (token == "public") {
				private_found = false;
				return (false);
			}
			if (token != "private:" && !private_found) {
				return (false);
			}
			return (true);
		}

		void	formatTypeHelper( std::string &token ) {
			token[0] = ::toupper(token[0]);
			size_t	pos = token.find('_');
			while (pos != std::string::npos) {
				token.erase(pos, 1);
				token[pos] = ::toupper(token[pos]);
				pos = token.find('_');
			}
		}

		void	formatTypes( void ) {
			for (it start = private_variables.begin(); start != private_variables.end(); start++) {
				formatTypeHelper(start->first);
			}
			for (it start = private_variables_compound.begin(); start != private_variables_compound.end(); start ++) {
				formatTypeHelper(start->first);
			}
		}

	public:
		Parse( const char *s ) : header_file(s), source_file("template.cpp") {  }
		Parse( const std::string &str ) : header_file(str), source_file("template.cpp") { }
		~Parse( void ) { }

		bool	readFile( void ) {
			std::string		line;
			std::string		token1;
			std::string		token2;
			std::ifstream	file(header_file.c_str());
			if (!file.is_open())
				return (false);
			while (std::getline(file, line)) {
				std::istringstream	tokens(line);
				if (line.find('(') != std::string::npos && line.find(')') != std::string::npos) {
					continue ;
				}
				else {
					std::getline(tokens, token1, '\t');
					std::getline(tokens, token2, '\t');
					if (!processTokens(token1)) {
						if (class_found) {
							class_found = false;
							class_name = token1;
						}
						continue ;
					}
					if (checkCompoundType(token1)) {
						private_variables_compound.push_back(std::make_pair(token1, token2));
					}
					else
						private_variables.push_back(std::make_pair(token1, token2));
				}
			}
			return (true);
		}

		bool	writeTemplate( void ) {
			formatTypes();
			std::string		output;
			for (it iter = private_variables.begin(); iter != private_variables.end(); iter ++) {
				output += iter->first;
				output += '\t';
				output += class_name;
				output += "::get";
				output += iter->second;
				output += "(  ) { }\n";
			}
			for (it iter = private_variables.begin(); iter != private_variables.end(); iter ++) {
				output += "const ";
				output += iter->first;
				output += "&\t";
				output += class_name;
				output += "::get";
				output += iter->second;
				output += "(  ) { }\n";
			}
			std::ofstream	file(source_file.c_str());
			if (!file.is_open())
				return (false);
			file << output;
			return (true);
		}

};

int	main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "need only one arugment as header file\n";
		return (1);
	}
	Parse p(av[1]);
	if (!p.readFile()) {
		std::cerr << "File cant open\n";
		return (1);
	}
	if (!p.writeTemplate()) {
		std::cerr << "File cant open\n";
		return (1);
	}
	return (0);
}