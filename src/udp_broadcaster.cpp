#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <set>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>

#include "udp_broadcaster.h"

using boost::asio::steady_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

udp_broadcaster::udp_broadcaster(boost::asio::io_context& io_context, const udp::endpoint& broadcast_endpoint): socket_(io_context){
	socket_.connect(broadcast_endpoint);
	socket_.set_option(udp::socket::broadcast(true));
}

void udp_broadcaster::deliver(const std::string& msg){
	boost::system::error_code ignored_ec;
	socket_.send(boost::asio::buffer(msg), 0, ignored_ec);
}