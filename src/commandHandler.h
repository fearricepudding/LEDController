#ifndef COMMANDHANDLER_H_INCLUDED
#define COMMANDHANDLER_H_INCLUDED

#define PIXELS 191 // # of pixels in strip

#include "ws2812-rpi.h"
#include <boost/thread.hpp>

struct RGB{
	short r;
	short g;
	short b;
};

class CommandHandler{
private:
	boost::thread* m_thread; 				// The thread runs this object						// The current frame number
	bool m_mustStop;						// Stop Command				// Current frame mutex
    boost::mutex m_mustStopMutex;			// Must stop mutex				// run next frame
	boost::thread* thr;						// This thread
	void processCommand();					// Process next command buffer
	std::string getNextCommand();
	int FromHex(const std::string&);
	RGB hex2rgb(std::string);
	std::vector<std::string> SplitWithCharacters(const std::string&, int);
	std::vector<Color_t> hexString2Color_t(std::string);

public:
	CommandHandler();							// Constructor
    ~CommandHandler();							// Destructor
    void Start();								// Start the thread
	void Stop();								// stop the thread
    void loop();								// Thread animation loop
    boost::mutex m_commandBuffer;				// Command buffer mutex
    std::vector<std::string> commandBuffer;		// Command buffer
};


#endif
