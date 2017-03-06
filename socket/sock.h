#pragma once

#include "export.h"
#include <WinSock2.h>


class EXPORT sock
{
    unsigned int fd;
    struct addrinfo hint;
    struct addrinfo * info;

public:

    enum TYPE { TCP, UDP };

    sock();

    unsigned int get_file_descriptor();
    struct addrinfo * get_addr_info();

    void set_socket_type(sock::TYPE type, int flags);
    void resolve_address_and_port( const char * addr, const char * port );

private:
    void init();
};


