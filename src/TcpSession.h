#ifndef H_TCPSESSION
#define H_TCPSESSION

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
#include "TcpChannel.h"

using boost::asio::steady_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class tcp_session: public subscriber, public boost::enable_shared_from_this<tcp_session>{
public:
	tcp_session(boost::asio::io_context& io_context, channel& ch);
	tcp::socket& socket();
	void start();

private:
	void stop();
	bool stopped() const;
	void deliver(const std::string& msg);
	void start_read();
	void handle_read(const boost::system::error_code& ec, std::size_t n);
	void await_output();
	void start_write();
	void handle_write(const boost::system::error_code& ec);
	void check_deadline(steady_timer* deadline);

	channel& channel_;
	tcp::socket socket_;
	std::string input_buffer_;
	steady_timer input_deadline_;
	std::deque<std::string> output_queue_;
	steady_timer non_empty_output_queue_;
	steady_timer output_deadline_;

};

typedef boost::shared_ptr<tcp_session> tcp_session_ptr;

#endif