#ifndef H_UDPBROADCASTER
#define H_UDPBROADCASTER

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

#include "TcpSubscriber.h"

using boost::asio::steady_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class udp_broadcaster: public subscriber{
public:
	udp_broadcaster(boost::asio::io_context& io_context, const udp::endpoint& broadcast_endpoint);

private:
	void deliver(const std::string& msg);
	udp::socket socket_;
};

#endif