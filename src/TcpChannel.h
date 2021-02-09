#ifndef H_TCPCHANNEL
#define H_TCPCHANNEL

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <set>
#include "TcpSubscriber.h"

class channel{
public: 
	void join(subscriber_ptr subscriber);
	void leave(subscriber_ptr subscriber);
	void deliver(const std::string& msg);

private: 
	std::set<subscriber_ptr> subscribers_;

};

#endif