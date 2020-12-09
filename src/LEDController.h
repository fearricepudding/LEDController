#ifndef LEDCONTROLLER_H_INCLUDED
#define LEDCONTROLLER_H_INCLUDED

#include "animationEngine.h"

class LEDController{
private: 
	static LEDController* instance;
public:
	static LEDController* getInstance();
	std::string newMessage(std::string);
	AnimationEngine *ac = new AnimationEngine();
	void startLoop();
	void stopLoop();
	void startListening();
};

#endif
