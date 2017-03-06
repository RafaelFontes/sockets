#include "sock.h"
#include <iostream>
#ifdef WIN32
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    WSADATA wsaData;
    bool wsaInitialized = false;

    // Need to link with Ws2_32.lib
    #pragma comment (lib, "Ws2_32.lib")
    // #pragma comment (lib, "Mswsock.lib")
#endif

sock::sock()
{
    init();
    fd = ~0;
}

unsigned int sock::get_file_descriptor()
{
    if ( fd == ~0 )
    {
        fd = socket( info->ai_family, info->ai_socktype, info->ai_protocol );

        if ( fd == ~0 )
        {
            freeaddrinfo( info );
            WSACleanup();

            throw "failed to create socket";
        }
    }

    return fd;
}

addrinfo *sock::get_addr_info()
{
    return info;
}

void sock::set_socket_type(sock::TYPE type, int flags)
{
    init();

    switch ( type )
    {
        case TCP:
            hint.ai_socktype = SOCK_STREAM;
            hint.ai_protocol = IPPROTO_TCP;
            break;
        case UDP:
            hint.ai_socktype = SOCK_DGRAM;
            hint.ai_protocol = IPPROTO_UDP;
            break;
    }

    hint.ai_flags = flags;
}

void sock::resolve_address_and_port(const char *addr, const char *port)
{
    if ( 0 != getaddrinfo( addr, port, &this->hint, &this->info ) )
    {
        throw "getaddrinfo failed";
    }
}

void sock::init()
{
#ifdef WIN32
    if ( !wsaInitialized )
    {
        WSAStartup( MAKEWORD(2,2), &wsaData );
        wsaInitialized = true;
    }
#endif;

    memset( &hint, 0, sizeof(hint) );
    hint.ai_family = AF_INET;
}
