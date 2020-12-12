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
	short r;
	short g;
	short b;
};

std::vector<std::string> SplitWithCharacters(const std::string& str, int splitLength) {
  int NumSubstrings = str.length() / splitLength;
  std::vector<std::string> ret;

  for (int i = 0; i < NumSubstrings; i++) {
     ret.push_back(str.substr(i * splitLength, splitLength));
  }

  // If there are leftover characters, create a shorter item at the end.
  if (str.length() % splitLength != 0) {
      ret.push_back(str.substr(splitLength * NumSubstrings));
  }


  return ret;
}


RGB hex2rgb(std::string hex) {
  RGB color;

  if(hex.at(0) == '#') {
      hex.erase(0, 1);
  }

  while(hex.length() != 6) {
      hex += "0";
  }

  std::vector<std::string> colori=SplitWithCharacters(hex,2);

  color.r = stoi(colori[0],nullptr,16);
  color.g = stoi(colori[1],nullptr,16);
  color.b = stoi(colori[2],nullptr,16);

  return color;
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
	// XXX: Hex commands
	// 0A start animation
	// 0B stop animation
	// 0C toggle LEDS
	std::string command = msg.substr(0, 2);
	
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
		std::string currentHex = hexString.substr(i, 6);
		RGB values = hex2rgb(currentHex);
		Color_t newColor(values.r, values.g, values.b);
		newBuffer.push_back(newColor);
	}
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

