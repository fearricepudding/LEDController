#include <boost/thread.hpp>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <boost/asio.hpp>
#include <sstream>
#include "animationEngine.h"
#include "LEDController.h"
#include "UdpThread.h"
#include "TcpServer.h"
#include "commandHandler.h"

LEDController* LEDController::instance = NULL;


void LEDController::start(){
	ac->Start();
    ch->Start();
    udpthread->start();
    startTcp();
}

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
	ch->m_commandBuffer.lock();
	ch->commandBuffer.push_back(msg);
	ch->m_commandBuffer.unlock();	
	std::cout << "!" << std::flush;
}

void LEDController::startTcp(){
	try{
		std::cout << "Starting tcp" << std::endl;
		boost::asio::io_context io_context;
		tcp::endpoint listen_endpoint(tcp::v4(), 999);
		udp::endpoint broadcast_endpoint(boost::asio::ip::make_address("0.0.0.0"), 9999);
		TcpServer s(io_context, listen_endpoint, broadcast_endpoint);
		io_context.run();
	}catch (std::exception& e){
		std::cerr << "Exception: " << e.what() << "\n";
	}
	std::cout << "End of tcp" << std::endl;
}

void LEDController::addNewFrame(std::vector<Color_t> newFrame){
	ac->m_frameBuffer.lock();
	ac->frameBuffer.push_back(newFrame);
	ac->m_frameBuffer.unlock();
	std::cout << "F" << std::flush;
}

void LEDController::toggle(){
	ac->toggle();
}

std::string LEDController::getStatus(){
	std::string status = ac->getStatus();
	return status;
}

int main(int argc, char* argv[]){
    std::cout << "Starting" << std::endl;
    LEDController *ledc = LEDController::getInstance();
    ledc->start();
	return 0;
}

