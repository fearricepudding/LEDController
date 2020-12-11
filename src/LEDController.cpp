#include <boost/thread.hpp>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <boost/asio.hpp>
#include <sstream>
#include "animationEngine.h"
#include "LEDController.h"
#include "tcp_server.h"
#include "udpServer.h"

LEDController* LEDController::instance = NULL;

LEDController* LEDController::getInstance()
{
   if (!instance)   // Only allow one instance of class to be generated.
      instance = new LEDController;
   return instance;
}

void LEDController::newMessage(std::string msg){
	std::cout << "new message: |" << msg << "|" << std::endl;
	
	if(msg == "stop"){
		ac->Stop();
	}
	else if(msg == "start"){
		ac->Start();
	}else{
		std::cout << "Unknown command" << std::endl;
	}
}

void LEDController::startLoop(){
    ac->Start();
}

void LEDController::stopLoop(){
	ac->Stop();
}

void LEDController::startListening(){
//	try{
//		boost::asio::io_context io_context;
//		tcp::endpoint listen_endpoint(tcp::v4(), 999);
//		udp::endpoint broadcast_endpoint(boost::asio::ip::make_address("0.0.0.0"), 9999);
//		server s(io_context, listen_endpoint, broadcast_endpoint);
//		io_context.run();
//	  }catch (std::exception& e){
//		std::cerr << "Exception: " << e.what() << "\n";
//	  }

	udp->listen();
}


int main(int argc, char* argv[]){
    std::cout << "Starting" << std::endl;

    LEDController *ledc = LEDController::getInstance();
    ledc->startLoop();
    ledc->startListening();
    
      
	return 0;
}

