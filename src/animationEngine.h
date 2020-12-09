#ifndef ANIMATIONENGINE_H_INCLUDED
#define ANIMATIONENGINE_H_INCLUDED

#include <boost/thread.hpp>

class AnimationEngine{
private:
	boost::thread* m_thread; // The thread runs this object
	int m_frame; // The current frame number
	bool m_mustStop;
    boost::mutex m_frameMutex;
    boost::mutex m_mustStopMutex;
	void DisplayNextFrame();
	boost::thread* thr;

public:
	// Default constructor
	AnimationEngine();
    ~AnimationEngine();
    void Start();
	void Stop();
    void animate();
};


#endif
