#pragma once

#include "export.h"

class buffer;

class EXPORT buffer_stack
{
    buffer_stack * next;
    buffer * value;

public:
    buffer_stack();

    void setValue( buffer * val );
    void setNext(buffer_stack * val );

    unsigned int getStackSize();

    buffer * getValue();

    buffer_stack * getNext();

    void destroy();
};
