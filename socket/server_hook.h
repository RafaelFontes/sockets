#pragma once

#include "export.h"

class EXPORT server_hook
{
public:

    virtual int read( void *, unsigned char *, unsigned int ) = 0;
    virtual int write( void *, unsigned char *, unsigned int ) = 0;
    virtual void close( void *) = 0;
};
