#include "AutoindexHandler.hpp"

/*
** ------------------------- CONSTRUCTOR & DESTRUCTOR --------------------------
*/

AutoindexHandler::AutoindexHandler() : _logger(Logger("AutoindexHandler"))
{
}

AutoindexHandler::~AutoindexHandler()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string AutoindexHandler::loadHtmlTemplate(void)
{
	std::ifstream file("autoindex/autoindex.html");
	std::stringstream autoindexTempl;

	if (!file.is_open())
	{
		std::cerr << "Failed to open HTML template file:" << std::endl;
		return "";
	}
	autoindexTempl << file.rdbuf();
	file.close();
	return autoindexTempl.str();
}

bool AutoindexHandler::checkIfHandle(const Request &request, RouteDetails &routeDetails, const std::string &fullPath)
{
	(void)request;
	if (routeDetails.autoindex == false)
	{
		_logger.log("Autoindex is disabled");
		return false;
	}
	if (!utl::isDirectory(fullPath))
	{
		_logger.log("Not a directory");
		return false;
	}
	std::string uri = request.getUri();
	bool uriMatchesRoute = (uri == routeDetails.route);
	if (!uriMatchesRoute)
	{
		_logger.log("URI does not match route");
		return false;
	}
	return true;
}

bool AutoindexHandler::handleRequest(const Request &req, Response &res, RouteDetails &routeDetails, const std::string &fullPath)
{
	DIR *dir;
	dirent *entry;
	std::ifstream file("./autoindex/autoindex.html");
	std::string autoindexTempl;
	std::string listingHtml;
	(void)routeDetails;

	autoindexTempl = loadHtmlTemplate();
	if (autoindexTempl.empty())
	{
		res.errorResponse(500, "Failed to load HTML template.");
		return true;
	}

	if ((dir = opendir(fullPath.c_str())) == NULL)
	{
		std::cerr << "Failed to open directory: " << fullPath << std::endl;
		res.setBody("<html><body><p>Error opening directory.</p></body></html>");
		return true;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue; // Skip . and ..

		std::string name = entry->d_name;
		std::string link = req.getRoute() + "/" + name;
		std::cout << "link: " << link << std::endl;
		listingHtml += "<li><a href='" + link + "'>" + name + "</a></li>\n";
	}
	closedir(dir);

	size_t pos = autoindexTempl.find("<!-- %DIRECTORY_CONTENTS% -->");
	if (pos != std::string::npos)
	{
		autoindexTempl.replace(pos, strlen("<!-- %DIRECTORY_CONTENTS% -->"), listingHtml);
	}

	res.setBody(autoindexTempl);
	res.setStatusCode(200);
	return true;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
