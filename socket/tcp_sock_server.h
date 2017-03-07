#pragma once

#include "export.h"
#include "sock.h"
#include <WS2tcpip.h>
#include <stdlib.h>
#include "tls_server_hook.h"
#include "sock_io.h"

class sock;

template<typename T>
class EXPORT tcp_sock_server
{
    sock * handler;
    T callback;
    tls_server_hook * tls;

    void bind(const char *addr, unsigned int port)
    {
        char buffer[10];
        memset(buffer, 0, 10);
        _itoa_s(port, buffer, 10, 10);

        handler->resolve_address_and_port( addr, buffer );

        int fd = handler->get_file_descriptor();

        struct addrinfo * info = handler->get_addr_info();

        if ( -1 == ::bind( fd, info->ai_addr, (int)info->ai_addrlen ) )
        {

            freeaddrinfo(info);
            closesocket(fd);
            WSACleanup();

            throw "failed to bind to socket";
        }
    }

    bool is_open;

    void wait_for_connection()
    {
        while ( is_open )
        {
            struct sockaddr_in addr;
                   socklen_t len = sizeof(addr);

            int client_fd = accept( handler->get_file_descriptor(), (struct sockaddr*)&addr, &len );

            if ( tls )
            {
               this->callback( new sock_io( client_fd, tls, tls->accept(client_fd) ) );
            }
            else
            {
                //@todo: handle no tls server
            }
        }
    }

public:
    explicit tcp_sock_server(T callback) : callback(callback)
    {
        tls = NULL;
        is_open = false;

        handler = new sock();
        handler->set_socket_type( sock::TCP, AI_PASSIVE );
    }

    ~tcp_sock_server()
    {
        delete handler;
    }

    void enableTLS( const char * cert, const char * key )
    {
        tls = new tls_server_hook( cert, key );
    }

    void listen(const char *addr, unsigned int port )
    {
        bind( addr, port );

        int fd = handler->get_file_descriptor();

        if ( -1 == ::listen( fd, SOMAXCONN ) )
        {
            closesocket(fd);
            WSACleanup();

            throw "failed to listen to socket";
        }

        is_open = true;

        wait_for_connection();
    }

};
