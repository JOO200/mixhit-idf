#include "MyWebServer.h"
#include "cCocktail.h"
#include "esp_log.h"
#include "RFID.h"
#include "../rfid/RFID_Bezahl.h"
#include "string.h"
#include <string>
#include <map>
#include "cJSON.h"
#include "esp_err.h"

MyWebServer::MyWebServer() {
	mServer = new HttpServer();
}

HttpServer * MyWebServer::getServer() {
	return mServer;
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
	static int currOrderNo = 0xA5;
	RfidData cocktail;
	memset(&cocktail, 0, sizeof(cocktail));
	if(!RFID_Bezahl::fromString(cocktail, request->getBody())) {
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Invalid JSON-Format");
		response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
		response->sendData("{\"Answer\": \"2\"}");
		response->close();
		return;
	}


	ESP_LOGI("Bestellung", "Received!");

	uint8_t cntr = 10, status;
	while ((status = RFID_Bezahl::writeData(cocktail)) != WRITE_SUCCESSFUL && --cntr > 0) {
		//response->delay();
		vTaskDelay(100);
	}
	cJSON * data = cJSON_CreateObject();
	// cJSON_AddNumberToObject(data, "cocktailNumber", cocktail.Bestellnummer);
	if(status == ESP_OK) {
		cJSON_AddStringToObject(data, "status", "Cocktail wurde gespeichert.");
	} else {
		cJSON_AddStringToObject(data, "status", "Fehler beim Schreiben.");
	}
	std::string payload = cJSON_Print(data);
	cJSON_Delete(data);
	ESP_LOGD("WebServer", "Sending payload %s", payload.c_str());
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	response->sendData(payload);
	response->close();

	/*Now try to write the data on the rfid-chip*/

	/* TODO: Statuszurückgabe an den Client:
	Idee: Die Antwort zu verzögern, bis die Bestellung auf den Chip geschrieben wird
	Zwischendrin muss dann halt noch ein entsprechender Statuscode gesendet werden, damit der Browser nicht in ein Timeout läuft.
	*/
}

void MyWebServer::indexRequest(HttpRequest * request, HttpResponse * response) {
	std::string fileName = WEB_ROOT_PATH "/html/index_redirect.html"; // Build the absolute file name to read.
	response->sendFile(fileName, 4 * 1024);			// Send File with default buffer size
	response->close();
}

void MyWebServer::listRequest(HttpRequest * request, HttpResponse * response) {
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = "/spiffs/config/cocktails.json";
	response->sendFile(fileName, 4 * 1024);
}
