#ifndef LEDCONTROLLER_H_INCLUDED
#define LEDCONTROLLER_H_INCLUDED

#include "UdpThread.h"
#include "animationEngine.h"

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
	UdpThread *udpthread = new UdpThread();			// UDP server
	void startLoop();								// start animation loop
	void stopLoop();								// stop animation loop
	void startListening();							// start tcp
	void startTcp();
};

#endif
