#pragma once

#include "export.h"
#include <mutex>

class buffer_stack;
class buffer;

class EXPORT buffer_controller
{
    unsigned int key;
    buffer_stack * first_stack;
    buffer_stack * last_stack;

    std::mutex locker;

    explicit buffer_controller(unsigned int key);

public:

    static buffer_controller * instance( unsigned int key );

    buffer_stack * stack();

    buffer * extract();

    void add( buffer * buf );
};
