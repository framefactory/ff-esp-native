/**
 * ESP/Native Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "WebServer.h"
#include "WebRouter.h"

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_err.h"

F_USE_NAMESPACE

static const char* TAG = "WebServer";

WebServer::WebServer() :
    _serverHandle(nullptr),
    _isRunning(false)
{
}

bool WebServer::start(uint16_t port)
{
    if (_isRunning) {
        return true;
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = port;

    if (httpd_start(&_serverHandle, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Server listening on port %d", port);
        _isRunning = true;
    }
    else {
        ESP_LOGI(TAG, "Failed to start server on port %d", port);
    }

    return _isRunning;
}

void WebServer::stop()
{
    if (_isRunning) {
        httpd_stop(_serverHandle);
        _serverHandle = nullptr;
        _isRunning = false;
    }
}

void WebServer::addRoute(const char* pUri, httpd_method_t method, WebRouter* pRouter)
{
    httpd_uri_t route = {
        .uri = pUri,
        .method = method,
        .handler = &WebServer::_requestHandler,
        .user_ctx = pRouter
    };

    httpd_register_uri_handler(_serverHandle, &route);
}

esp_err_t WebServer::_requestHandler(httpd_req_t* pRequest)
{
    WebRouter* pRouter = static_cast<WebRouter*>(pRequest->user_ctx);
    esp_err_t result = ESP_OK;

    if (pRouter) {
        result = pRouter->onRequest(pRequest) ? ESP_OK : ESP_FAIL;
    }

    return result;
}