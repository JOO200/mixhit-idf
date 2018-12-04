#ifndef _MY_WEBSERVER_H_
#define _MY_WEBSERVER_H_

#include "HttpServer.h"
#include "../rfid/RFID_Bezahl.h"

#define WEB_ROOT_PATH "/spiffs/web"

class MyWebServer {
public:
	MyWebServer();
	void start();
	HttpServer * getServer();
private:
	static void indexRequest(HttpRequest * request, HttpResponse * response);
	static void listRequest(HttpRequest * request, HttpResponse * response);
	static void makeCocktail(HttpRequest * request, HttpResponse * response);
	HttpServer * mServer;
};
extern RFID_Bezahl bezahl;

#endif // _MY_WEBSERVER_H_
