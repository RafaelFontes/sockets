#pragma once

#include "server_hook.h"
#include "export.h"
#include <openssl/ssl.h>

class EXPORT tls_server_hook : public server_hook
{
    SSL_CTX * ssl_context;

public:
    tls_server_hook( const char * cert, const char * key );
    ~tls_server_hook();

    SSL * accept( int client_fd );

    void destroy(SSL* ssl);

    int read( void* ssl, unsigned char * buf, unsigned int size );

    int write( void *ssl, unsigned char * buf, unsigned int size );

    void close(void *ssl);
};
