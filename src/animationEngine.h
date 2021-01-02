#ifndef ANIMATIONENGINE_H_INCLUDED
#define ANIMATIONENGINE_H_INCLUDED

#define PIXELS 191 // # of pixels in strip

#include "ws2812-rpi.h"
#include <boost/thread.hpp>

struct RGB{
	short r;
	short g;
	short b;
};

class AnimationEngine{
private:
	boost::thread* m_thread; 				// The thread runs this object
	int m_frame; 							// The current frame number
	bool m_mustStop;						// Stop Command
    boost::mutex m_frameMutex;				// Current frame mutex
    boost::mutex m_mustStopMutex;			// Must stop mutex
	void DisplayNextFrame();				// run next frame
	boost::thread* thr;						// This thread
	NeoPixel *strip = new NeoPixel(PIXELS);	// LED strip controller
	void processCommand();					// Process next command buffer
	std::string getNextCommand();
	int FromHex(const std::string&);
	RGB hex2rgb(std::string);
	std::vector<std::string> SplitWithCharacters(const std::string&, int);
	std::vector<Color_t> hexString2Color_t(std::string);

public:
	AnimationEngine();							// Constructor
    ~AnimationEngine();							// Destructor
    void Start();								// Start the thread
	void Stop();								// stop the thread
    void animate();								// Thread animation loop
    void replaceBuffer(std::vector<Color_t>);	// Replace LED buffer
    void update();								// Update lights to LED buffer
    boost::mutex m_commandBuffer;				// Command buffer mutex
    std::vector<std::string> commandBuffer;		// Command buffer
};


#endif
