#ifndef LEDCONTROLLER_H_INCLUDED
#define LEDCONTROLLER_H_INCLUDED

class LEDController{
private: 
	static LEDController* instance;					// singleton instance
public:
	static LEDController* getInstance();			// return singleton
	std::string newMessage(std::string);			// recieve message from tcp
	AnimationEngine *ac = new AnimationEngine();	// animation loop
	void startLoop();								// start animation loop
	void stopLoop();								// stop animation loop
	void startListening();							// start tcp
};

#endif
