#ifndef H_TCPTHREAD
#define H_TCPTHREAD

#include <boost/thread.hpp>

class TcpThread{
public: 
	TcpThread();
	~TcpThread();
	void start();
	void stop();
	boost::mutex m_mustStopMutex;
	boost::thread* m_thread; 	
	
private:
	void loop();
	void listen();
	bool m_stop;

};

#endif