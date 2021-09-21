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
#include "LEDController.h"
#include "helpers.h"

AnimationEngine::AnimationEngine(){
    m_thread=NULL;
    m_mustStop=false;
    m_frame=0;
}

AnimationEngine::~AnimationEngine(){
    if (m_thread!=NULL) m_thread->join();
}

void AnimationEngine::replaceBuffer(std::vector<Color_t> newBuffer){
	strip->setBrightness(1.);
	strip->LEDBuffer = newBuffer;
	update();
}

void AnimationEngine::update(){
	strip->show();
}

void AnimationEngine::DisplayNextFrame(){
    // Simulate next frame
    m_frameBuffer.lock();
    if(frameBuffer.size() <= 0){
        m_frameBuffer.unlock();
        return;
    }
    m_frameBuffer.unlock();
    std::vector<Color_t> newFrame = getNextFrame();
    replaceBuffer(newFrame);
    int currentFrame = 0;
    m_frameMutex.lock();
    m_frame++;
    currentFrame = m_frame;
    m_frameMutex.unlock();      
}

std::vector<Color_t> AnimationEngine::getNextFrame(){
    std::vector<Color_t> newFrame;
    m_frameBuffer.lock();
    if(frameBuffer.size() > 0){
        newFrame = frameBuffer[0];
        frameBuffer.erase(frameBuffer.begin());
    }
    m_frameBuffer.unlock();
    return newFrame;
}

void AnimationEngine::toggle(){
    if(strip->state){
        strip->state = false;
    }else{
        strip->state = true;
    }
    update();
}

void AnimationEngine::Start(){
	m_mustStopMutex.lock();
    m_mustStop=false;
    m_mustStopMutex.unlock();
    m_thread = new boost::thread(&AnimationEngine::animate, this);
}

void AnimationEngine::Stop(){
    m_mustStopMutex.lock();
    m_mustStop=true;
    m_mustStopMutex.unlock();
    if (m_thread!=NULL) {
        m_thread->join();
    };
}

void AnimationEngine::animate(){
    bool mustStop = false;
    while(!mustStop){
        DisplayNextFrame();
        // Sleep thread to limit framerate
        boost::this_thread::sleep(boost::posix_time::millisec(40));
        m_mustStopMutex.lock();
        mustStop=m_mustStop;
        m_mustStopMutex.unlock();
    };
}


std::string AnimationEngine::getStatus(){
    std::map<std::string, std::string> response;
    response["state"] = strip->state;
    std::cout << LEDC::helpers::stringify(response) << std::endl;
    std::string temp = "GOOD";
    return temp;
}