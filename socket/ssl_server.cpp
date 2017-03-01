#include "ssl_server.h"

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define FAIL    -1

SSL_CTX *ctx;


void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
    printf("Server certificates:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Subject: %s\n", line);
    free(line);       /* free the malloc'ed string */
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    free(line);       /* free the malloc'ed string */
    X509_free(cert);     /* free the malloc'ed certificate copy */
}
else
    printf("No certificates.\n");

    flushall();
}

void Servlet(SSL* ssl) /* Serve the connection -- threadable */
{
    char buf[1024];
    char reply[1024];
    int bytes;
    const char* HTMLecho="<html><body><pre>%s</pre></body></html>\n\n";

    std::cout << "Do sslprotocol accept!" << std::endl;
    if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
    {

        std::cout << "not accepted!" << std::endl;

        ERR_print_errors_fp(stderr);
    }
    else
    {
        ShowCerts(ssl);        /* get any certificates */
        bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
        if ( bytes > 0 )
        {
            buf[bytes] = 0;
            std::cout << "Client msg: " << buf << std::endl;

            sprintf(reply, HTMLecho, buf);   /* construct reply */
            SSL_write(ssl, reply, strlen(reply)); /* send reply */
        }
        else
        {
            std::cout << "No bytes!!" << std::endl;

            ERR_print_errors_fp(stderr);
        }
    }
    SSL_free(ssl);         /* release SSL state */
}


ssl_server::ssl_server()
{

}

void ssl_server::init()
{
    std::cout << "init!" << std::endl;

    SSL_library_init();

    std::cout << "add algorithms!" << std::endl;

    OpenSSL_add_all_algorithms();

    std::cout << "load errors!" << std::endl;

    SSL_load_error_strings();

    std::cout << "creating ctx!" << std::endl;

    ctx = SSL_CTX_new(SSLv23_server_method());

    SSL_CTX_set_ecdh_auto(ctx, 1);

    std::cout << "loading cert!" << std::endl;

    if ( SSL_CTX_use_certificate_file(ctx, "G:/andre/server/bin/server.cert", SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    std::cout << "loading key!" << std::endl;

    if ( SSL_CTX_use_PrivateKey_file(ctx, "G:/andre/server/bin/server.key", SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }

    std::cout << "checking key!" << std::endl;
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }

    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    std::cout << "All good!" << std::endl;
}

int ssl_server::start()
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    while(1)
    {
        struct sockaddr_in addr;
               socklen_t len = sizeof(addr);
               SSL *ssl;
        const char reply[] = "Ola q tal?\n";

        ClientSocket = accept(ListenSocket, (struct sockaddr*)&addr, &len);
        std::cout << "New connection: " <<  inet_ntoa(addr.sin_addr) << "::" << ntohs(addr.sin_port) << std::endl;

        std::cout << "get new ssl state with context" << std::endl;
        ssl = SSL_new(ctx);              /* get new SSL state with context */

        std::cout << "Setting connection socket to SSL state" << std::endl;
        SSL_set_fd(ssl, ClientSocket);      /* set connection socket to SSL state */

        Servlet(ssl);
        closesocket(ClientSocket);
    }

    closesocket(ListenSocket);

    SSL_CTX_free(ctx);
    closesocket(ClientSocket);
    WSACleanup();

    return 0;

}
