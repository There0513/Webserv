#include <map>
#include <string>

void    MimeTypeInit() {
    std::map<std::string, std::string>  MimeTypes;

    MimeTypes["aac"] = "audio/aac";
	MimeTypes["abw"] = "application/x-abiword";
	MimeTypes["arc"] = "application/octet-stream";
	MimeTypes["avi"] = "video/x-msvideo";
	MimeTypes["azw"] = "application/vnd.amazon.ebook";
	MimeTypes["bin"] = "application/octet-stream";
	MimeTypes["bmp"] = "image/bmp";
	MimeTypes["bz"] = "application/x-bzip";
	MimeTypes["bz2"] = "application/x-bzip2";
	MimeTypes["csh"] = "application/x-csh";
	MimeTypes["css"] = "text/css";
	MimeTypes["csv"] = "text/csv";
	MimeTypes["doc"] = "application/msword";
	MimeTypes["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	MimeTypes["eot"] = "application/vnd.ms-fontobject";
	MimeTypes["epub"] = "application/epub+zip";
	MimeTypes["gif"] = "image/gif";
	MimeTypes["htm"] = "text/html";
	MimeTypes["html"] = "text/html";
	MimeTypes["ico"] = "image/x-icon";
	MimeTypes["ics"] = "text/calendar";
	MimeTypes["jar"] = "application/java-archive";
	MimeTypes["jpeg"] = "image/jpeg";
	MimeTypes["jpg"] = "image/jpeg";
	MimeTypes["js"] = "application/javascript";
	MimeTypes["json"] = "application/json";
	MimeTypes["mid"] = "audio/midi";
	MimeTypes["midi"] = "audio/midi";
	MimeTypes["mpeg"] = "video/mpeg";
	MimeTypes["mpkg"] = "application/vnd.apple.installer+xml";
	MimeTypes["odp"] = "application/vnd.oasis.opendocument.presentation";
	MimeTypes["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	MimeTypes["odt"] = "application/vnd.oasis.opendocument.text";
	MimeTypes["oga"] = "audio/ogg";
	MimeTypes["ogv"] = "video/ogg";
	MimeTypes["ogx"] = "application/ogg";
	MimeTypes["otf"] = "font/otf";
	MimeTypes["png"] = "image/png";
	MimeTypes["pdf"] = "application/pdf";
	MimeTypes["ppt"] = "application/vnd.ms-powerpoint";
	MimeTypes["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	MimeTypes["rar"] = "application/x-rar-compressed";
	MimeTypes["rtf"] = "application/rtf";
	MimeTypes["sh"] = "application/x-sh";
	MimeTypes["svg"] = "image/svg+xml";
	MimeTypes["swf"] = "application/x-shockwave-flash";
	MimeTypes["tar"] = "application/x-tar";
	MimeTypes["tif"] = "image/tiff";
	MimeTypes["tiff"] = "image/tiff";
	MimeTypes["ts"] = "application/typescript";
	MimeTypes["ttf"] = "font/ttf";
	MimeTypes["txt"] = "text/plain";
	MimeTypes["vsd"] = "application/vnd.visio";
	MimeTypes["wav"] = "audio/x-wav";
	MimeTypes["weba"] = "audio/webm";
	MimeTypes["webm"] = "video/webm";
	MimeTypes["webp"] = "image/webp";
	MimeTypes["woff"] = "font/woff";
	MimeTypes["woff2"] = "font/woff2";
	MimeTypes["xhtml"] = "application/xhtml+xml";
	MimeTypes["xls"] = "application/vnd.ms-excel";
	MimeTypes["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	MimeTypes["xml"] = "application/xml";
	MimeTypes["xul"] = "application/vnd.mozilla.xul+xml";
	MimeTypes["zip"] = "application/zip";
	MimeTypes["3gp"] = "audio/3gpp";
	MimeTypes["3g2"] = "audio/3gpp2";
	MimeTypes["7z"] = "application/x-7z-compressed";
}

std::string     StatusCodeInit(int code) {
    std::map<int, std::string>  StatusCodes;
    
    StatusCodes[100] = "Continue";
    StatusCodes[101] = "Switching Protocols";
    StatusCodes[200] = "OK";
    StatusCodes[201] = "Created";
    StatusCodes[202] = "Accepted";
    StatusCodes[203] = "Non-Authoritative Information";
    StatusCodes[204] = "No Content";
    StatusCodes[205] = "Reset Content";
    StatusCodes[206] = "Partial Content";
    StatusCodes[300] = "Multiple Choices";
    StatusCodes[301] = "Moved Permanently";
    StatusCodes[302] = "Found";
    StatusCodes[303] = "See Other";
    StatusCodes[304] = "Not Modified";
    StatusCodes[305] = "Use Proxy";
    StatusCodes[307] = "Temporary Redirect";
    StatusCodes[400] = "Bad Request";
    StatusCodes[401] = "Unauthorized";
    StatusCodes[402] = "Payment Required";
    StatusCodes[403] = "Forbidden";
    StatusCodes[404] = "Not Found";
    StatusCodes[405] = "Method Not Allowed";
    StatusCodes[406] = "Not Acceptable";
    StatusCodes[407] = "Proxy Authentication Required";
    StatusCodes[408] = "Request Time-out";
    StatusCodes[409] = "Conflict";
    StatusCodes[410] = "Gone";
    StatusCodes[411] = "Length Required";
    StatusCodes[412] = "Precondition Failed";
    StatusCodes[413] = "Request Entity Too Large";
    StatusCodes[414] = "Request-URI Too Large";
    StatusCodes[415] = "Unsupported Media Type";
    StatusCodes[416] = "Requested range not satisfiable";
    StatusCodes[417] = "Expectation Failed";
    StatusCodes[500] = "Internal Server Error";
    StatusCodes[501] = "Not Implemented";
    StatusCodes[502] = "Bad Gateway";
    StatusCodes[503] = "Service Unavailable";
    StatusCodes[504] = "Gateway Time-out";
    StatusCodes[505] = "HTTP Version not supported";
    return StatusCodes[code];
}

std::string     getDate() {
    char    date[33];
	size_t  size;
    time_t  now = time(0);

	tm *gmtm = gmtime(&now);

	size = strftime(date, 32, "%a, %d %b %Y %T GMT", gmtm);
	date[size] = '\0';
	return (std::string(date));
}
