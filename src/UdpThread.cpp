#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sys/types.h>

#include "UdpThread.h"
#include "udpServer.h"

UdpThread::UdpThread(){
	m_thread = NULL;
	m_stop = false;
}

UdpThread::~UdpThread(){};

void UdpThread::listen(){
	udpsrv->listen();
}

void UdpThread::start(){
	m_mustStopMutex.lock();
	m_stop = false;
	m_mustStopMutex.unlock();
	m_thread = new boost::thread(&UdpThread::loop, this);
}

void UdpThread::stop(){
	m_mustStopMutex.lock();
	m_stop = true;
	m_mustStopMutex.unlock();
	if(m_thread!=NULL){
		m_thread->join();
	}
}

void UdpThread::loop(){
	bool mustStop = false;
	while(!mustStop){
		listen();
		m_mustStopMutex.lock();
		mustStop = m_stop;
		m_mustStopMutex.unlock();
	}
}