/**
 * ESP/Native Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_NET_WEBSERVER_H
#define _ESP_NATIVE_NET_WEBSERVER_H

#include "../library.h"
#include "esp_http_server.h"

F_BEGIN_NAMESPACE

class WebRouter;

class WebServer
{
public:
    WebServer();
    virtual ~WebServer() {}

    bool start(uint16_t port);
    void stop();

    void addRoute(const char* pUri, httpd_method_t method, WebRouter* pRouter);
    httpd_handle_t handle() const { return _serverHandle; }

private:
    static esp_err_t _requestHandler(httpd_req_t* pRequest);
    httpd_handle_t _serverHandle;
    bool _isRunning;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_NET_WEBSERVER_H