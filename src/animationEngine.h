#ifndef ANIMATIONENGINE_H_INCLUDED
#define ANIMATIONENGINE_H_INCLUDED

#define PIXELS 191 // # of pixels in strip

#include "ws2812-rpi.h"
#include <boost/thread.hpp>

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

public:
	AnimationEngine();		// Constructor
    ~AnimationEngine();		// Destructor
    void Start();			// Start the thread
	void Stop();			// stop the thread
    void animate();			// Thread animation loop
    void replaceBuffer(std::vector<Color_t>);
    void update();
};


#endif
