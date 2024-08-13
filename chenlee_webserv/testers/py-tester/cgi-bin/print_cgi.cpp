#include <stdio.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char **argv, char **env)
{
	std::string body;
	char buffer[1000];
	int bytes;
	if ((bytes = read(0, buffer, sizeof buffer)) > 0)
		body = std::string(buffer, bytes);
	return 0;
}
