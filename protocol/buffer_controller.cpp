#include "buffer_controller.h"
#include <stdlib.h>
#include <map>
#include <mutex>
#include "buffer.h"
#include "buffer_stack.h"
#include <iostream>

std::map<unsigned int, buffer_controller*> controllers;

buffer_controller::buffer_controller(unsigned int key) : key(key)
{
    std::cout << "-> buffer_controller::buffer_controller" << std::endl;

    first_stack = 0;
    last_stack = 0;

    std::cout << "<- buffer_controller::buffer_controller" << std::endl;

}

buffer_controller *buffer_controller::instance( unsigned int key )
{
    std::cout << "-> buffer_controller::instance" << std::endl;

    if ( 0 == controllers.count( key ) )
    {
        controllers.insert( std::pair<unsigned int, buffer_controller*>( key, new buffer_controller( key ) ) );
    }

    auto ctrl = controllers.at( key );

    std::cout << "<- buffer_controller::instance" << std::endl;

    return ctrl;
}

buffer_stack *buffer_controller::stack()
{
    return first_stack;
}

buffer *buffer_controller::extract()
{
    std::cout << "-> buffer_controller::extract" << std::endl;

    buffer_stack * stack = first_stack;

    unsigned int totalSize = stack->getStackSize();
    unsigned char * fullBuffer = new unsigned char[totalSize];
    unsigned char * walker = fullBuffer;

    while ( stack )
    {
        auto current = stack->getValue();

        memcpy( walker, current->getData(), current->getSize() );

        walker += current->getSize();

        //auto current_stack = stack;

        stack = stack->getNext();

        //current_stack->destroy();
    }

    auto buf = new buffer( fullBuffer, totalSize );

    std::cout << "<- buffer_controller::extract" << std::endl;

    return buf;
}

void buffer_controller::add(buffer *buf)
{
    std::cout << "-> buffer_controller::add" << std::endl;

    locker.lock();

    std::cout << ":: locked" << std::endl;


    if ( !last_stack )
    {
        first_stack =
        last_stack = new buffer_stack();

        last_stack->setValue(buf);
        first_stack = last_stack;
    }
    else
    {
        buffer_stack * current = new buffer_stack();
        current->setValue( buf );

        last_stack->setNext( current );
        last_stack = current;
    }

    std::cout << ":: unlocking" << std::endl;

    locker.unlock();

    std::cout << "<- buffer_controller::add" << std::endl;
}
