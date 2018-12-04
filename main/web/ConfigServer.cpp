#include "ConfigServer.h"
#include "esp_log.h"
#include "cJSON.h"
#include <stdio.h>
#include <regex>

#include "../config/ConfigProvider.h"
#include "../config/WebConfig.h"

#define CONFIG_PATH "/spiffs/config"

ConfigServer::ConfigServer() {
}

void ConfigServer::start(HttpServer * server) {
	// mServer->setRootPath(WEB_PATH);
	std::string configPath = ConfigProvider::getPath(eConfig_WebConfig);
	if(configPath.empty()) {
		ESP_LOGE("ConfigServer", "No config file found.");
		return;
	}
	if(!WebConfig::adminMode(configPath)) {
		ESP_LOGI("ConfigServer", "Config server is disabled.");
		ESP_LOGI("ConfigServer", "To enable config server, reflash spiffs.");
		return;
	}
	/*server->addPathHandler(HttpRequest::HTTP_METHOD_GET,
		"/",
		&ConfigServer::configIndex);*/
	server->addPathHandler(HttpRequest::HTTP_METHOD_GET,	// ConfigList
		"/configs",
		&ConfigServer::onConfigListRequest);
	server->addPathHandler(HttpRequest::HTTP_METHOD_POST,
		"/config",
		&ConfigServer::onConfigRequest);
	server->addPathHandler(HttpRequest::HTTP_METHOD_POST,	// Config Save
		"/config_save",
		&ConfigServer::onConfigSaveRequest);
	std::regex * r = new std::regex("(.*)json");
	server->addPathHandler(HttpRequest::HTTP_METHOD_GET,
			r,
			&ConfigServer::onJsonRequest);
}

/* STATIC */ void ConfigServer::onJsonRequest(HttpRequest * request, HttpResponse * response)
{
	std::string fileName = std::string(CONFIG_PATH) + request->getPath(); // Build the absolute file name to read.
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	response->sendFile(fileName, 4*1024);
}

/* STATIC */ void ConfigServer::onConfigListRequest(HttpRequest * request, HttpResponse * response)
{
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = CONFIG_PATH "/configs.json";
	response->sendFile(fileName, 4 * 1024);
}

/* STATIC */ void ConfigServer::configIndex(HttpRequest * request, HttpResponse * response)
{
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	std::string fileName = "/spiffs/web/html/config_redirect.html";
	response->sendFile(fileName, 4 * 1024);
}

/* STATIC */ void ConfigServer::onConfigRequest(HttpRequest * request, HttpResponse * response)
{
	std::string json = request->getBody();
	cJSON * root = cJSON_Parse(json.c_str());
	if (root == NULL || !cJSON_HasObjectItem(root, "file")) {
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Invalid Json");
		response->close();
		ESP_LOGE("ConfigServer", "Invalid request., JSON invalid: %s", json.c_str());
		if (root != NULL) cJSON_Delete(root);
		return;
	}
	std::string file = cJSON_GetObjectItem(root, "file")->valuestring;
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = CONFIG_PATH;
	if(file.at(0) != '/') fileName += "/";
	fileName += file.c_str();
	response->sendFile(fileName, 4 * 1024);
	cJSON_Delete(root);
}

/* STATIC */ void ConfigServer::onConfigSaveRequest(HttpRequest * request, HttpResponse * response)
{
	ESP_LOGI("ConfigServer", "Saving config:");
	std::string json = request->getBody();
	cJSON * root = cJSON_Parse(json.c_str());
	if (!cJSON_HasObjectItem(root, "Pfad") || !cJSON_HasObjectItem(root, "Config"))
	{
		ESP_LOGE("ConfigServer", "Invalid JSON received.");
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Invalid Json");
		response->close();
		if (root != NULL) cJSON_Delete(root);
		return;
	}
	std::string file = cJSON_GetObjectItem(root, "Pfad")->valuestring;
	std::string path = CONFIG_PATH;
	if(file.at(0) != '/') path += "/";
	path += file;
	ESP_LOGI("ConfigServer", "Saving config %s", path.c_str());

	cJSON * config = cJSON_GetObjectItem(root, "Config");
	std::string configString = cJSON_Print(config);
	if (root != NULL) cJSON_Delete(root);	// Wir sind hier fertig mit allem JSON-Kram.
	FILE * f = fopen(path.c_str(), "wb");
	if (f == NULL) {
		ESP_LOGE("ConfigServer", "Error writing file.");
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Error writing file.");
		response->close();
		return;
	}
	fprintf(f, configString.c_str());
	fclose(f);
	ESP_LOGI("ConfigServer", "Saved config %s", path.c_str());
}
