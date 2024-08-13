#include "Location.hpp"

void Location::display() const {
    std::cout << "Location: " << path << std::endl
              << " Root: " << root << std::endl
              << " Index: " << index << std::endl
              << " CGI Pass: " << cgi_pass << std::endl
              << " Autoindex: " << (autoindex ? "on" : "off") << std::endl;
    if (!allowed_methods.empty()) {
        std::cout << " Allowed Methods: ";
        for (size_t i = 0; i < allowed_methods.size(); ++i) {
            std::cout << allowed_methods[i] << " ";
        }
        std::cout << std::endl;
    }
}
