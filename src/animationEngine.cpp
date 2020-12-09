#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include "animationEngine.h"
#include "tcp_server.h"
#include "ws2812-rpi-defines.h"
#include <sys/types.h>
#include "ws2812-rpi.h"

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
    int currentFrame = 0;
    m_frameMutex.lock();
    m_frame++;
    currentFrame = m_frame;
    m_frameMutex.unlock();
    
    //XXX FRAME ENGINE
    // XXX: TODO: Actual frame engine...
    
    
    // XXX: TEMP SHOW
     uint16_t i, j;
/*
//rainbow cycle

    for(j=0; j<256*5; j++) {
        for(i=0; i < strip->numPixels(); i++) {
            strip->setPixelColor(i, strip->wheel(((i * 256 / strip->numPixels()) + j) & 255));
        }
        strip->show();
    }
*/
	std::vector<Color_t> colours = strip->getPixels();

    for(i=1; i < strip->numPixels(); i++) {
    	colours[i] = colours[i-1];
    };
    strip->LEDBuffer = colours;
    strip->show();
        
    //XXX END OF FRAME ENGINE
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
