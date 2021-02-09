#ifndef H_TCPTHREAD
#define H_TCPTHREAD

#include <boost/thread.hpp>

#include "udpServer.h"

class UdpThread{
public: 
	UdpThread();
	~UdpThread();
	void start();
	void stop();
	boost::mutex m_mustStopMutex;
	boost::thread* m_thread; 	
	
private:
	void loop();
	void listen();
	bool m_stop;
	UdpServer *udpsrv = new UdpServer();

};

#endif