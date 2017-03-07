#include "packet.h"
#include "buffer.h"
#include <string.h>

#define BIT_1 1
#define BIT_2 2
#define BIT_3 4
#define BIT_4 8

#define FLAGS_POSITION 0
#define IDENTIFIER_POSITION 2
#define LENGTH_POSITION 6
#define FULL_SIZE_POSITION 10
#define TOKEN_SIZE_POSITION 12

bool bigEndian = false;

packet::packet()
{
    defaults();

    unsigned int endianess = 0;
    unsigned char test[4] = {0,0,0,1};
    memcpy( &endianess, test, 4 );

    bigEndian = ( 1 == endianess );
}

packet::~packet()
{
    data->destroy();
    delete data;

    defaults();
}

void packet::setBuffer(buffer *val)
{
    data = val;

    extractFlags();
    extractNumbers();
}

void packet::defaults()
{
    data = 0;
    token = 0;
    body = 0;
    containsToken = false;
    multibyte = false;
    requiresResponse = false;
    keepAlive = false;

    lenght = ~0;
    fullSize = ~0;
    tokenSize = ~0;
    id = ~0;
}

void packet::extractNumbers()
{
    unsigned int bodyPos = 0;

    if ( data && data->getSize() > 5 )
    {
        unsigned char *numbers  = data->getData();
        memcpy(&id, &numbers[IDENTIFIER_POSITION], 4 );
        memcpy(&lenght, &numbers[LENGTH_POSITION], 4 );
        memcpy(&fullSize, &numbers[FULL_SIZE_POSITION], 4);

        bodyPos = FULL_SIZE_POSITION+4;

        swapBytes( lenght );
        swapBytes( fullSize );
        swapBytes( id );

        if ( containsToken )
        {
            memcpy(&tokenSize, &numbers[TOKEN_SIZE_POSITION], 4 );
            swapBytes( tokenSize );

            if ( tokenSize > 0 )
            {
                token = new unsigned char[tokenSize];
                memcpy( token, &numbers[TOKEN_SIZE_POSITION+4], tokenSize );
            }

            bodyPos += 4 + tokenSize;
        }

        if ( lenght > 0 )
        {
            body = new unsigned char[ lenght ];

            memcpy( body, &numbers[bodyPos], lenght );

        }
    }
}

void packet::swapBytes( unsigned int &destination )
{
    if ( !bigEndian )
    {
        unsigned char walker[4];
        unsigned char output[4];

        memcpy( walker, &destination, 4);

        for ( unsigned int i = 0; i < 4; ++i )
        {
            output[i] = walker[3 - i];
        }
        memcpy( &destination, &output, 4);
    }
}

void packet::extractFlags()
{
    if ( data && data->getSize() > FLAGS_POSITION )
    {
        unsigned char * flags = data->getData();

        multibyte        = 0 != ( (int)flags[FLAGS_POSITION] & BIT_1 );
        containsToken    = 0 != ( (int)flags[FLAGS_POSITION] & BIT_2 );
        requiresResponse = 0 != ( (int)flags[FLAGS_POSITION] & BIT_3 );
        keepAlive        = 0 != ( (int)flags[FLAGS_POSITION] & BIT_4 );
    }
}

