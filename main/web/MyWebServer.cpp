#include "MyWebServer.h"
#include "cCocktail.h"
#include "esp_log.h"

MyWebServer::MyWebServer() {
	mServer = new HttpServer();
}

void MyWebServer::start() {
	mServer->setRootPath(WEB_ROOT_PATH);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,
			"/index.html",
			&MyWebServer::indexRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,
			"/",
			&MyWebServer::indexRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,
			"/cocktails",
			&MyWebServer::listRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_POST,
			"/cocktail",
			&MyWebServer::makeCocktail);
	mServer->start(80);
}

void MyWebServer::makeCocktail(HttpRequest * request, HttpResponse * response) {
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	response->sendData("{\"Answer\": \"1\"}");
	response->close();

	std::string body = request->getBody();		// Body ist ein Cocktail JSON
	ESP_LOGI("Webserver", "test"); //, body.c_str());
	ESP_LOGI("Webserver", "%s", body.c_str());
	/*cCocktail cocktail =*/ cCocktail::fromJson(body);
}

void MyWebServer::indexRequest(HttpRequest * request, HttpResponse * response) {
	std::string fileName = WEB_ROOT_PATH "/html/index_redirect.html"; // Build the absolute file name to read.
	response->sendFile(fileName, 4 * 1024);			// Send File with default buffer size
}

void MyWebServer::listRequest(HttpRequest * request, HttpResponse * response) {
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = WEB_ROOT_PATH "/cocktails.json";
	response->sendFile(fileName, 4 * 1024);
	response->close();
}
