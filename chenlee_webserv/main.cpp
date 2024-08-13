#include "Webserver.hpp"
#include "ConfigParser.hpp"
#include "AutoindexHandler.hpp"
#include "CGIHandler.hpp"
#include "RedirectionHandler.hpp"

void testRouter(void)
{
	Router router("/home/sirhcofe/Core/webserv");
	router.addRoute("/jaclyn", new RouteDetails(false, "index.html", "/public/jaclyn", GET));
	router.addRoute("/post", new RouteDetails(false, "index.html", "/public/post", GET | POST));
	std::vector<RequestHandler *> requestHandlers;
	requestHandlers.push_back(new CGIHandler());
	requestHandlers.push_back(new AutoindexHandler());
	requestHandlers.push_back(new StaticFileHandler());
	requestHandlers.push_back(new RedirectionHandler());
	router.assignHandlers(requestHandlers);

	Request req("GET /jaclyn/./haha/popo/.. HTTP/1.1\r\nHOST: localhost:8080");
	Response res;
	router.routeRequest(req, res);

	Request req2("POST /post/hello.txt HTTP/1.1\r\nHOST: localhost:8080");
	Response res2;
	router.routeRequest(req2, res2);
}

void testAutoindex(void)
{
	Request req("GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");
	Response res;
	AutoindexHandler autoindexHandler;
	RouteDetails routeDetails;
	routeDetails.root = "/Users/user/sidess/webserv/public";
	autoindexHandler.handleRequest(req, res, routeDetails, "/Users/user/sidess/webserv/public");
}

void testCGI(void)
{
	Request req("POST /directory/youpi.bla HTTP/1.1\r\n\r\n");
	Response res;
	CGIHandler cgiHandler;
	RouteDetails routeDetails;
	routeDetails.allowedMethods = POST;
	routeDetails.cgiPass = "CGI-tester";
	cgiHandler.handleRequest(req, res, routeDetails, "/Users/user/sidess/webserv/YoupiBanane/youpi.bla");
}

void testWebserver(std::string configFile)
{
	// WebServer webserver(PORT, "localhost", "Webserver");

	// webserver.start();
	ConfigParser configParser;
	std::vector<RequestHandler *> requestHandlers;
	requestHandlers.push_back(new CGIHandler());
	requestHandlers.push_back(new AutoindexHandler());
	requestHandlers.push_back(new StaticFileHandler());
	requestHandlers.push_back(new RedirectionHandler());
	std::vector<ServerBlock *> serverBlocks;

	configParser.createServerBlocksFromConf("conf/" + configFile, requestHandlers, serverBlocks);
	for (std::vector<ServerBlock *>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
	{
		configParser.displayConfig(**it);
	}

	Webserver webserver(serverBlocks);
	webserver.start();
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <file.conf>" << std::endl;
		exit(1);
	}

	// testRouter();
	// testAutoindex();
	// testCGI();
	testWebserver(std::string(argv[1]));
}
