#include <set>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>

#include "TcpServer.h"
#include "TcpSubscriber.h"
#include "TcpSession.h"
#include "udp_broadcaster.h"

TcpServer::TcpServer(boost::asio::io_context& io_context,
    const tcp::endpoint& listen_endpoint,
	const udp::endpoint& broadcast_endpoint
):io_context_(io_context), acceptor_(io_context, listen_endpoint){
  	subscriber_ptr bc(new udp_broadcaster(io_context_, broadcast_endpoint));
    channel_.join(bc);
    start_accept();
}

void TcpServer::start_accept(){
	tcp_session_ptr new_session(new tcp_session(io_context_, channel_));
    acceptor_.async_accept(new_session->socket(), boost::bind(&TcpServer::handle_accept, this, new_session, _1));
}

void TcpServer::handle_accept(tcp_session_ptr session, const boost::system::error_code& ec){
	if (!ec){
      session->start();
    }
    start_accept();
}

