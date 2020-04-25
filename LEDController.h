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
        // states
        bool ready;
        bool state;
        RGB colour;
        // Methods
        bool check();
        bool toggle();
        bool toggle(bool state);
        RGB status();
};

#endif // LEDCONTROLLER_H
