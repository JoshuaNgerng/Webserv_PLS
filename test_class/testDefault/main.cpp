/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jngerng <jngerng@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 18:32:20 by jngerng           #+#    #+#             */
/*   Updated: 2024/10/28 18:43:37 by jngerng          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"
#include "DefaultErrorPage.hpp"

// c++ *.cpp -o testing

int main(void) {
    AutoIndex           a;
    DefaultErrorPage    err;
    std::string autoindex_html = a.generateResource("../../public");
    std::string erropage_html = err.generateHtml(401, "testing");
    std::ofstream   f1("test_autoindex.html");
    std::ofstream   f2("test_errorpage.html");
    f1 << autoindex_html;
    f2 << erropage_html;
    return (0);
}
