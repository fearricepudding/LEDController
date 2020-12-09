#include <boost/thread.hpp>
#include <iostream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <boost/asio.hpp>
#include "animationEngine.h"
#include "tcp_server.h"

AnimationEngine::AnimationEngine(){
    m_thread=NULL;
    m_mustStop=false;
    m_frame=0;
}

AnimationEngine::~AnimationEngine(){
    if (m_thread!=NULL) m_thread->join();
}

void AnimationEngine::DisplayNextFrame(){
    // Simulate next frame
    m_frameMutex.lock();
    m_frame++;
    m_frameMutex.unlock();
    std::cout << "Frame" << std::endl;
}

void AnimationEngine::Start(){
	m_mustStopMutex.lock();
    m_mustStop=false;
    m_mustStopMutex.unlock();
    m_thread = new boost::thread(&AnimationEngine::animate, this);
}

void AnimationEngine::Stop(){
    std::cout << "STOPPING" << std::endl;

//	// Signal the thread to stop (thread-safe)
    m_mustStopMutex.lock();
    m_mustStop=true;
    m_mustStopMutex.unlock();

    // Wait for the thread to finish.
    if (m_thread!=NULL) m_thread->join();
}

void AnimationEngine::animate(){
    bool mustStop = false;
    std::cout << "Must stop: " << mustStop << std::endl;
    while(!mustStop){
        // Display the next animation frame
        DisplayNextFrame();
        // Sleep for 40ms (25 frames/second).
        boost::this_thread::sleep(boost::posix_time::millisec(40));
        // Get the "must stop" state (thread-safe)
        m_mustStopMutex.lock();
        mustStop=m_mustStop;
        m_mustStopMutex.unlock();
    };
}
