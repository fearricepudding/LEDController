#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <set>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "TcpChannel.h"
#include "TcpSubscriber.h"

void channel::join(subscriber_ptr subscriber){
	subscribers_.insert(subscriber);
}

void channel::leave(subscriber_ptr subscriber){
	subscribers_.erase(subscriber);
}

void channel::deliver(const std::string& msg){
	std::for_each(subscribers_.begin(), subscribers_.end(), boost::bind(&subscriber::deliver, _1, boost::ref(msg)));
}