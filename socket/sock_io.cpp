#include "sock_io.h"
#include "server_hook.h"
#include <WinSock2.h>
#include <protocol/packet.h>
#include <protocol/buffer.h>

sock_io::sock_io(unsigned int fd, server_hook *hook, void * user_data ) : hook(hook), user_data(user_data), fd(fd)
{
    u_long io_blocking_disabled = 0;

    ioctlsocket( fd, FIONBIO, &io_blocking_disabled );
    is_socket_open = true;
}

void sock_io::read( packet &p )
{
    static const int size = 1450;

    unsigned char buf[size];

    memset(buf, 0, size);

    int bytesAvailable = this->hook->read( user_data, buf, size );

    p.setBuffer( new buffer( buf, bytesAvailable ) );
}

int sock_io::read(unsigned char *buf, unsigned int size)
{
    return this->hook->read( user_data, buf, size );
}

int sock_io::write(unsigned char *buf, unsigned int size)
{
    return this->hook->write( user_data, buf, size );
}

bool sock_io::is_open()
{
    return is_socket_open;
}

void sock_io::close()
{
    this->hook->close( user_data );
}

unsigned int sock_io::get_file_descriptor()
{
    return fd;
}
