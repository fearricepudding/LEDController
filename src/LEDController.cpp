#include <boost/thread.hpp>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <boost/asio.hpp>
#include <sstream>
#include "animationEngine.h"
#include "LEDController.h"
#include "udpServer.h"

LEDController* LEDController::instance = NULL;


std::string LEDController::ToHex(const std::string& s, bool upper_case){
    std::ostringstream ret;
    for (std::string::size_type i = 0; i < s.length(); ++i){
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];
    };
    return ret.str();
}

int LEDController::FromHex(const std::string &s){
	return strtoul(s.c_str(), NULL, 16); 
}


LEDController* LEDController::getInstance(){
   if (!instance){
      instance = new LEDController;
   };
   return instance;
}

void LEDController::newMessage(std::string msg){
	// XXX: Add comand to "AE" command buffer
	ac->m_commandBuffer.lock();
	ac->commandBuffer.push_back(msg);
	ac->m_commandBuffer.unlock();	
	std::cout << "!" << std::flush;
}


void LEDController::startLoop(){
    ac->Start();
}

void LEDController::stopLoop(){
	ac->Stop();
}

void LEDController::startListening(){
	udp->listen();
}

int main(int argc, char* argv[]){
    std::cout << "Starting" << std::endl;
    LEDController *ledc = LEDController::getInstance();
    ledc->startLoop();
    ledc->startListening();
	return 0;
}