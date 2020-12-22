/**
 * ESP/Native Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_NET_WEBROUTER_H
#define _ESP_NATIVE_NET_WEBROUTER_H

#include "../library.h"
#include "WebServer.h"

F_BEGIN_NAMESPACE

class WebRouter
{
public:
    WebRouter(WebServer* pServer);
    virtual ~WebRouter() { }

    virtual bool onRequest(httpd_req_t* pRequest) = 0;

    WebServer* server() const { return _pServer; }

protected:

private:
    WebServer* _pServer;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_NET_WEBROUTER_H