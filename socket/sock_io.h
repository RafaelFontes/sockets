#pragma once

class server_hook;
class packet;

class sock_io
{
    server_hook * hook;
    void * user_data;
    unsigned int fd;

    bool is_socket_open;

public:
    explicit sock_io(unsigned int fd, server_hook * hook , void * user_data);

    void read( packet & p );

    int read( unsigned char * buf, unsigned int size );
    int write( unsigned char * buf, unsigned int size );
    void close();

    unsigned int get_file_descriptor();
    bool is_open();
};
