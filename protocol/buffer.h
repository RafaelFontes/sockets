#pragma once

#include "export.h"

class EXPORT buffer
{
    unsigned char * data;
    unsigned int size;

public:
    buffer( unsigned char * data, unsigned int size );
    ~buffer();

    void destroy();

    unsigned char * getData();
    unsigned int getSize();
};
