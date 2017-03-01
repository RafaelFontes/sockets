#pragma once

#include <server/sock_io.h>
#include <server/tcp_sock_server.h>

void sock_test()
{
    auto onConnect = []( sock_io * io ) {

        unsigned char buff[255] = {0};

        io->read( buff, 255 );

        std::cout << buff << std::endl;

        int bytesSent = io->write( (unsigned char*)"HTTP 200 OK\n\n<h1>Hello!</h1>\n\n\n\n", 33);

        /**
          * (2 first bytes)
          * [bits:]
          *
          * 1 : single/multi packet
          * 2 : packet contains token
          * 3 : requires response
          * 4 : keeps connection alive after response
          * 5 - 16: reserved bits
          *
          * [bytes:]
          * 3rd - 4th byte: packet length ( if its multi packet, current packet size )
          * 5th - 12th byte: full packet size
          * 13th - 16th: token size if available
          * 17th ~ token if available
          * ~ body
          */

        io->close();
    };

    auto server = new tcp_sock_server<decltype(onConnect)>( onConnect );
         server->enableTLS("G:/andre/server/bin/server.cert","G:/andre/server/bin/server.key");
         server->listen("0.0.0.0", 8081);
}
