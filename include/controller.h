#include "ws2812-rpi.h"

#define PIXELS 191
class controller{
public:
	NeoPixel *n = new NeoPixel(PIXELS);
	controller();
	void toggle();
	void start();
};
