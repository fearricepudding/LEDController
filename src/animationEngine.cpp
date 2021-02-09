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
	strip->setBrightness(1.);
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
        
}

std::vector<std::string> AnimationEngine::SplitWithCharacters(const std::string& str, int splitLength) {
    int NumSubstrings = str.length() / splitLength;
    std::vector<std::string> ret;
    for (int i = 0; i < NumSubstrings; i++) {
        ret.push_back(str.substr(i * splitLength, splitLength));
    };
    // If there are leftover characters, create a shorter item at the end.
    if (str.length() % splitLength != 0) {
        ret.push_back(str.substr(splitLength * NumSubstrings));
    };
    return ret; 
}

RGB AnimationEngine::hex2rgb(std::string hex) {
    RGB color;
    if(hex.at(0) == '#') {
        hex.erase(0, 1);
    };
    while(hex.length() != 6) {
        hex += "0";
    };
    std::vector<std::string> colori=SplitWithCharacters(hex,2);
    color.r = stoi(colori[0],nullptr,16);
    color.g = stoi(colori[1],nullptr,16);
    color.b = stoi(colori[2],nullptr,16);
    return color;
}

std::vector<Color_t> AnimationEngine::hexString2Color_t(std::string hexString){
    std::vector<Color_t> newBuffer; 
    for(int i = 0; i < (hexString.length()); i+=6){
        std::string currentHex = hexString.substr(i, 6);
        RGB values = hex2rgb(currentHex);
        Color_t newColor(values.r, values.g, values.b);
        newBuffer.push_back(newColor);
    };
    return newBuffer;
}

std::string AnimationEngine::getNextCommand(){
    std::string newCommand;
    m_commandBuffer.lock();
    if(commandBuffer.size() > 0){
        newCommand = commandBuffer[0];
        commandBuffer.erase(commandBuffer.begin());
    }
    m_commandBuffer.unlock();
    return newCommand;
}

void AnimationEngine::processCommand(){
    std::cout << "." << std::flush;
    m_commandBuffer.lock();
    if(commandBuffer.size() == 0){
        m_commandBuffer.unlock();
        return;
    }
    m_commandBuffer.unlock();
    std::cout << "Command" << std::endl;
    std::string msg = getNextCommand();
    std::string command = msg.substr(0, 2);
    if(command == "0A"){
        if(strip->state){
            strip->state = false;
        }else{
            strip->state = true;
        }
        update();
    }else if(command == "0D"){
        std::string newBuffer = msg.substr(2, msg.length()-1);
        // confirm we have the right amount of data
        int newBufferLength = newBuffer.length();
        if((newBufferLength/6) != 191){
            std::cout << "New buffer incorrect size. expected 1146 recieved " << newBufferLength << std::endl;
            return;
        };
        std::vector<Color_t> newColorBuffer = hexString2Color_t(newBuffer);
        replaceBuffer(newColorBuffer);

    }else{
        std::cout << "Command not found" << std::endl;
    };

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
        processCommand();
        DisplayNextFrame();
        // Sleep thread to limit framerate
        boost::this_thread::sleep(boost::posix_time::millisec(40));
        m_mustStopMutex.lock();
        mustStop=m_mustStop;
        m_mustStopMutex.unlock();
    };
}
