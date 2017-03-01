#include "sock_io.h"
#include "server_hook.h"
#include <WinSock2.h>

sock_io::sock_io(int fd, server_hook *hook, void * user_data ) : hook(hook), user_data(user_data), fd(fd)
{

}

int sock_io::read(unsigned char *buf, unsigned int size)
{
    return this->hook->read( user_data, buf, size );
}

int sock_io::write(unsigned char *buf, unsigned int size)
{
    return this->hook->write( user_data, buf, size );
}

void sock_io::close()
{
    this->hook->close( user_data );
    //closesocket(fd);
}
