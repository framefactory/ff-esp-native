/**
 * ESP/Native Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "WebRouter.h"

F_USE_NAMESPACE

WebRouter::WebRouter(WebServer* pServer) :
    _pServer(pServer)
{
}