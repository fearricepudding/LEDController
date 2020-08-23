#include "ws2812-rpi.h"

#define PIXELS 191

int main(int argc, char **argv){
    NeoPixel *n=new NeoPixel(PIXELS);
//    int i = 0;
//    n->setBrightness(.5f);
//    for(i=0;i < PIXELS; i++){
//
//	    n->setPixelColor(i, 100, 150, 100);
//    }
//    n->show();
	n->setBrightness(.3f);
	while(true){
	    	n->rainbowCycle(5);
	}
    delete n;
    return 0;
}
