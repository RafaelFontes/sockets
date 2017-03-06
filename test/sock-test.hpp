#pragma once

#include <server/sock_io.h>
#include <server/tcp_sock_server.h>
#include <protocol/buffer_controller.h>
#include <protocol/buffer_stack.h>
#include <protocol/buffer.h>
#include <chrono>
#include <thread>


void sock_test()
{
    auto onConnect = []( sock_io * io ) {

        unsigned int fd = io->get_file_descriptor();
        unsigned char buff[1500] = {0};

        int size= 0;

        std::cout << "Nova conexao :: fd -> " << fd << std::endl;

        auto controller = buffer_controller::instance( fd );

        while( 1 )
        {
            memset( buff, 0, 1500 );

            int readBytes = io->read( buff, 1500 );

            std::cout << "Kbei de ler " <<readBytes<< " bytes" << std::endl;

            size += readBytes;

            if ( readBytes > 0 )
            {
                auto curBuffer = new buffer( buff, readBytes );

                controller->add( curBuffer );

                std::cout << buff << " added to the buffer " << std::endl;

            }

            if ( size >= 10 )
            {
                break;
            }

            std::this_thread::sleep_for( std::chrono::milliseconds(33) );
        }

        auto full = controller->extract();

        std::cout << "Mensagem completa:" << std::string( (char*)full->getData(), full->getSize() ).c_str() << std::endl;

        full->destroy();

        controller->stack()->destroy();
        /*std::cout << "got " << readBytes << "bytes";

        std::cout << "content:" << buff << std::endl;

        int bytesSent = io->write( (unsigned char*)"HTTP 200 OK\n\n<h1>Hello!</h1>\n\n\n\n", 33);
*/
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
          * 3rd - 6th byte: packet identifier
          * 7rd - 8th byte: packet length ( if its multi packet, current packet size )
          * 9th - 12th byte: full packet size
          * 13th - 16th: token size if available
          * 17th ~ token if available
          * ~ body
          */

        io->close();
    };

    auto server = new tcp_sock_server<decltype(onConnect)>( onConnect );
         server->enableTLS("server.crt","server.key");
         server->listen("0.0.0.0", 8081);

}
