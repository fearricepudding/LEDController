#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sys/types.h>

#include "tcpServer.h"

TcpThread::TcpThread(){
	m_thread = NULL;
	m_stop = false;
}

TcpThread::~TcpThread(){};

void TcpThread::listen(){

}

void TcpThread::start(){
	m_mustStopMutex.lock();
	m_stop = false;
	m_mustStopMutex.unlock();
	m_thread = new boost::thread(&TcpThread::loop, this);
}

void TcpThread::stop(){
	m_mustStopMutex.lock();
	m_stop = true;
	m_mustStopMutex.unlock();
	if(m_thread!=NULL){
		m_thread->join();
	}
}

void TcpThread::loop(){
	bool mustStop = false;
	while(!mustStop){
		listen();
		m_mustStopMutex.lock();
		mustStop = m_stop;
		m_mustStopMutex.unlock();
	}
}