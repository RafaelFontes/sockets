#pragma once
#include <protocol/packet.h>
#include <server/sock_io.h>
#include <server/tcp_sock_server.h>
#include <protocol/buffer_controller.h>
#include <protocol/buffer_stack.h>
#include <protocol/buffer.h>
#include <protocol/packet.h>
#include <protocol/json.h>
#include <chrono>
#include <thread>


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

using json::JSON;

void sock_test()
{
    auto onConnect = []( sock_io * io ) {

        while( 1 )
        {
            packet income_packet;

            io->read( income_packet );

            if ( income_packet.lenght > 0 )
            {
                JSON payload = JSON::Load( income_packet );

                if ( payload.hasKey("action") )
                {
                    if ( payload["action"] == "say" )
                    {
                        for ( int i = 0; i < payload["args"].length(); ++i )
                        {
                            std::cout << i << " : " << payload["args"][i] << std::endl;
                        }
                    }

                    std::cout << payload  << std::endl;
                }
            }

            std::this_thread::sleep_for( std::chrono::milliseconds(33) );
        }

        io->close();
    };

    auto server = new tcp_sock_server<decltype(onConnect)>( onConnect );
         server->enableTLS("server.crt","server.key");
         server->listen("0.0.0.0", 8081);

}
