#pragma once

#include "export.h"

class EXPORT ssl_server
{
public:
    ssl_server();
    void init();
    int start();
};
