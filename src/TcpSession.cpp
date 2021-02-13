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

#include "TcpSession.h"
#include "TcpSubscriber.h"
#include "TcpChannel.h"
#include "LEDController.h"

using boost::asio::steady_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

tcp_session::tcp_session(boost::asio::io_context& io_context, channel& ch)
    : channel_(ch),
      socket_(io_context),
      input_deadline_(io_context),
      non_empty_output_queue_(io_context),
      output_deadline_(io_context)
  {
    input_deadline_.expires_at(steady_timer::time_point::max());
    output_deadline_.expires_at(steady_timer::time_point::max());

    // The non_empty_output_queue_ steady_timer is set to the maximum time
    // point whenever the output queue is empty. This ensures that the output
    // actor stays asleep until a message is put into the queue.
    non_empty_output_queue_.expires_at(steady_timer::time_point::max());
  }

tcp::socket& tcp_session::socket(){
	return socket_;
}

void tcp_session::start(){
	std::cout << "New Session Started" << std::endl;
	channel_.join(shared_from_this());
	start_read();
	input_deadline_.async_wait(boost::bind(&tcp_session::check_deadline, shared_from_this(), &input_deadline_));
	await_output();
	output_deadline_.async_wait(boost::bind(&tcp_session::check_deadline, shared_from_this(), &output_deadline_));
}

void tcp_session::stop(){
	channel_.leave(shared_from_this());
	boost::system::error_code ignored_ec;
	socket_.close(ignored_ec);
	input_deadline_.cancel();
	non_empty_output_queue_.cancel();
	output_deadline_.cancel();
}

bool tcp_session::stopped() const{
	return !socket_.is_open();
}

void tcp_session::deliver(const std::string& msg){
	output_queue_.push_back(msg+"\n");
	non_empty_output_queue_.expires_at(steady_timer::time_point::min());
}

void tcp_session::start_read(){
	input_deadline_.expires_after(boost::asio::chrono::seconds(30));
	boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(input_buffer_), "\n", boost::bind(&tcp_session::handle_read, shared_from_this(), _1, _2));
}

void tcp_session::handle_read(const boost::system::error_code& ec, std::size_t n){
	if(stopped()){
		return;
	}
	if(!ec){
		std::string msg(input_buffer_.substr(0, n-1));
		input_buffer_.erase(0, n);
		if(!msg.empty()){
			LEDController *ledc = LEDController::getInstance();
			//XXX: Message not empty process data!!

			std::cout << std::endl << "[*] " << msg << std::endl;
			if(msg == "status"){
				std::string status = ledc->getStatus();
				channel_.deliver(status);
			}else{
				channel_.deliver("READY"); // XXX: Reponse	
			};
		}else{
			if(output_queue_.empty()){
				output_queue_.push_back("\n");
				non_empty_output_queue_.expires_at(steady_timer::time_point::min());
			}
		}
		start_read();
	}else{
		stop();
	}
}

void tcp_session::await_output(){
	if(stopped()){
		return;
	}
	if(output_queue_.empty()){
		non_empty_output_queue_.expires_at(steady_timer::time_point::max());
		non_empty_output_queue_.async_wait(boost::bind(&tcp_session::await_output, shared_from_this()));
	}else{
		start_write();
	}
}

void tcp_session::start_write(){
	output_deadline_.expires_after(boost::asio::chrono::seconds(30));
	boost::asio::async_write(socket_, boost::asio::buffer(output_queue_.front()), boost::bind(&tcp_session::handle_write, shared_from_this(), _1));
}

void tcp_session::handle_write(const boost::system::error_code& ec){
	if(stopped()){
		return;
	}
	if(!ec){
		output_queue_.pop_front();
		await_output();
	}else{
		stop();
	}
}

void tcp_session::check_deadline(steady_timer* deadline){
	if(stopped()){
		return;
	}
	if(deadline->expiry() <= steady_timer::clock_type::now()){
		stop();
	}else{
		deadline->async_wait(boost::bind(&tcp_session::check_deadline, shared_from_this(), deadline));
	}
}
