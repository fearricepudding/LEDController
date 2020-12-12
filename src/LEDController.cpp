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



struct RGB{
	unsigned int r;
	unsigned int g;
	unsigned int b;
};


struct RGB LEDController::colorConverter(int hexValue){
  struct RGB rgbColor;
  rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
  rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
  rgbColor.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte

  return rgbColor; 
}



//-----------------

std::string ToHex(const std::string& s, bool upper_case /* = true */)
{
    std::ostringstream ret;

    for (std::string::size_type i = 0; i < s.length(); ++i)
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];

    return ret.str();
}

int FromHex(const std::string &s) { return strtoul(s.c_str(), NULL, 16); }

//-----------------



LEDController* LEDController::instance = NULL;

LEDController* LEDController::getInstance()
{
   if (!instance)   // Only allow one instance of class to be generated.
      instance = new LEDController;
   return instance;
}

void LEDController::newMessage(std::string msg){
	//std::cout << "new message: |" << msg << "|" << std::endl;
	// XXX: Hex commands
	// 0A start animation
	// 0B stop animation
	// 0C toggle LEDS
	std::string command = msg.substr(0, 2);
	
	std::cout << "|" << command << "|" << std::endl;
	
	if(command == "0A"){
		std::cout << "start anim" << std::endl;
		
		//ac->toggle();
	}
	else if(command == "0B"){
		std::cout << "stop anim" << std::endl;
	}
	else if(command == "0C"){
		std::cout << "Toggle LEDs" << std::endl;
	}
	else if(command == "0D"){
		std::cout << "swap buffer" << std::endl;
		std::string newBuffer = msg.substr(2, msg.length()-1); // get payload
		// confirm we have all data
		int newBufferLength = newBuffer.length();
		if((newBufferLength/6) != 191){
			std::cout << "New buffer incorrect size. expected 1146 recieved " << newBufferLength << std::endl;
			return;
		}
		std::vector<Color_t> newColorBuffer = hexString2Color_t(newBuffer);
		ac->replaceBuffer(newColorBuffer);
	}
	else
	{
		std::cout << "Command not found" << std::endl;
	};
	
}

std::vector<Color_t> LEDController::hexString2Color_t(std::string hexString){
	std::vector<Color_t> newBuffer; 
	for(int i = 0; i < (hexString.length()); i+=6){
		std::string currentHex = hexString.substr(i, i+6);
		unsigned int hexint;
		std::stringstream tmp;
		tmp << std::hex << currentHex;
		tmp >> hexint;
		RGB values = colorConverter(hexint);
		Color_t newColor(values.r, values.g, values.b);
		newBuffer.push_back(newColor);
	}
	std::cout << newBuffer.size() << std::endl;
	return newBuffer;
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
    //ledc->startLoop();
    ledc->startListening();
	return 0;
}

