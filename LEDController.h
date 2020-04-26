#include "libs.h"
#include <pigpio.h>

#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

struct RGB {
    int R;
    int G;
    int B;
};

class LEDController
{
    public:
        LEDController();

        // states
        bool ready;
        bool state;
        RGB colour;
        // Methods
        bool check();
        bool toggle();
        bool toggle(bool state);
        RGB status();


    private:
        bool update();
};

#endif // LEDCONTROLLER_H
