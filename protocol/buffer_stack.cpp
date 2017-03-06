#include "buffer_stack.h"
#include "buffer.h"
#include <iostream>

buffer_stack::buffer_stack()
{
    value = 0;
    next = 0;
}

void buffer_stack::setValue(buffer *val)
{
    value = val;
}

void buffer_stack::setNext(buffer_stack *val)
{
    next = val;
}

unsigned int buffer_stack::getStackSize()
{
    buffer_stack * stack = this;

    unsigned int size = 0;

    while ( stack )
    {
        size += stack->getValue()->getSize();
        stack = stack->next;
    }

    return size;
}

buffer *buffer_stack::getValue()
{
    return value;
}

buffer_stack *buffer_stack::getNext()
{
    return next;
}

void buffer_stack::destroy()
{
    std::cout << "-> buffer_stack::destroy" << std::endl;

    buffer_stack * stack = this;

    while ( stack )
    {
        auto current = stack->value;
             current->destroy();
        delete current;

        auto current_stack = stack;
        stack = stack->next;

        delete current_stack;
    }

    std::cout << "<- buffer_stack::destroy" << std::endl;

}
