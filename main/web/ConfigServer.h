#ifndef _CONFIGSERVER_H_
#define _CONFIGSERVER_H_

#include "HttpServer.h"

class ConfigServer {
public:
	ConfigServer();
	void start(HttpServer * mServer);
private:
	static void configIndex(HttpRequest * request, HttpResponse * response);
	static void onJsonRequest(HttpRequest * request, HttpResponse * response);
	static void onConfigRequest(HttpRequest * request, HttpResponse * response);
	static void onConfigListRequest(HttpRequest * request, HttpResponse * response);
	static void onConfigSaveRequest(HttpRequest * request, HttpResponse * response);

};

#endif // _CONFIGSERVER_H_
