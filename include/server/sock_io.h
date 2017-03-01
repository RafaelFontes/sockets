#pragma once

class server_hook;

class sock_io
{
    server_hook * hook;
    void * user_data;
    int fd;

public:
    explicit sock_io(int fd, server_hook * hook , void * user_data);

    int read( unsigned char * buf, unsigned int size );
    int write( unsigned char * buf, unsigned int size );
    void close();
};
