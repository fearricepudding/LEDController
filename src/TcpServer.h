#ifndef H_TCPSERVER
#define H_TCPSERVER

#include <set>
#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>

#include "TcpSession.h"

using boost::asio::steady_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class TcpServer {
public: 
	TcpServer(boost::asio::io_context& io_context,
		const tcp::endpoint& listen_endpoint,
		const udp::endpoint& broadcast_endpoint);
	void start_accept();
	void handle_accept(tcp_session_ptr session,
		const boost::system::error_code& ec);

private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
	channel channel_;
};

#endif