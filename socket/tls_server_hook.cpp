#include "tls_server_hook.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

tls_server_hook::tls_server_hook(const char *cert, const char *key) : server_hook()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ssl_context = SSL_CTX_new(SSLv23_server_method());

    if ( !ssl_context )
    {
        throw "unable to create ssl context";
    }

    SSL_CTX_set_ecdh_auto(ssl_context, 1);

    if ( SSL_CTX_use_certificate_file(ssl_context, cert, SSL_FILETYPE_PEM) <= 0 )
    {
        throw "failed to load certificate";
    }

    if ( SSL_CTX_use_PrivateKey_file(ssl_context, key, SSL_FILETYPE_PEM) <= 0 )
    {
        throw "failed to load private key";
    }

    if ( !SSL_CTX_check_private_key(ssl_context) )
    {
        throw "private key does not match the public certificate";
    }
}

tls_server_hook::~tls_server_hook()
{
    SSL_CTX_free(ssl_context);
}

SSL *tls_server_hook::accept(int client_fd)
{
    SSL *ssl = SSL_new(ssl_context);

    SSL_set_fd( ssl, client_fd );
    SSL_accept( ssl );

    return ssl;
}

void tls_server_hook::destroy(SSL *ssl)
{
    SSL_shutdown(ssl);
    SSL_free(ssl);
}

int tls_server_hook::read(void *ssl, unsigned char *buf, unsigned int size)
{
    return SSL_read( (SSL*)ssl, buf, size );
}

int tls_server_hook::write(void *ssl, unsigned char *buf, unsigned int size)
{
    return SSL_write( (SSL*)ssl, buf, size );
}

void tls_server_hook::close( void *ssl)
{
    this->destroy( (SSL*)ssl );
}
