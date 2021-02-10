#ifndef LEDCONTROLLER_H_INCLUDED
#define LEDCONTROLLER_H_INCLUDED

#include <sys/types.h>

#include "UdpThread.h"
#include "animationEngine.h"
#include "ws2812-rpi.h"
#include "commandHandler.h"

class LEDController{
private: 
	static LEDController* instance;					
	struct RGB colorConverter(int hexValue);	
	std::string ToHex(const std::string&, bool);
	int FromHex(const std::string&);

public:
	static LEDController* getInstance();			// return singleton
	void newMessage(std::string);			        // recieve message from tcp
	AnimationEngine *ac = new AnimationEngine();	// animation loop
	CommandHandler *ch = new CommandHandler();
	UdpThread *udpthread = new UdpThread();			// UDP server
	void startTcp();
	void addNewFrame(std::vector<Color_t> newFrame);
	void toggle();
	void start();
};

#endif
