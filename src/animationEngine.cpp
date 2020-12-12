#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include "animationEngine.h"
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

void AnimationEngine::replaceBuffer(std::vector<Color_t> newBuffer){
	std::cout << "New Buffer" << std::endl;
	strip->setBrightness(1.0);
	strip->LEDBuffer = newBuffer;
	update();
}

void AnimationEngine::update(){
	strip->show();
}

void AnimationEngine::DisplayNextFrame(){
    // Simulate next frame
    int currentFrame = 0;
    m_frameMutex.lock();
    m_frame++;
    currentFrame = m_frame;
    m_frameMutex.unlock();
    
    // XXX: TODO: Actual animating
    
//    Color_t curPixel;
//    for(int i = 1; i < strip->numPixels(); i++){
//    	curPixel = strip->getPixelColor(i);
//    	strip->setPixelColor(i-1, curPixel);
//    }
//    strip->show();
        
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
