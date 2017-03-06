#include "buffer.h"
#include <string.h>
#include <iostream>

buffer::buffer(unsigned char *data, unsigned int size) : size(size)
{
    std::cout << "-> buffer::buffer created:: " << (void*)this << std::endl;

    if ( size > 0 )
    {
        this->data = new unsigned char[size];
        memcpy(this->data, data, size );
    }

    std::cout << "<- buffer::buffer" << std::endl;
}

buffer::~buffer()
{
    std::cout << "-> buffer::~buffer" << std::endl;

    if ( size > 0 )
    {
        delete data;
        data = 0;
        size = 0;
    }

    std::cout << "<- buffer::~buffer" << std::endl;

}

void buffer::destroy()
{    
    std::cout << "-> buffer::destroy :: " << size << " bytes to free" << std::endl;

    if ( size > 0 )
    {
        delete data;
        data = 0;
        size = 0;
    }

    std::cout << "<- buffer::destroy :: " << (void*)this << std::endl;

}

unsigned char *buffer::getData()
{
    return data;
}

unsigned int buffer::getSize()
{
    return size;
}
