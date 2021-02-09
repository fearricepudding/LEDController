#ifndef H_TCPSUBSCRIBER
#define H_TCPSUBSCRIBER

class subscriber{
public:
	virtual ~subscriber(){};
	virtual void deliver(const std::string& msg) = 0;
};

typedef boost::shared_ptr<subscriber> subscriber_ptr;

#endif