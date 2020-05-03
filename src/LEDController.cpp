#include "LEDController.h"
#include "main.h"

bool LEDController::check()
{
    return true;
}

RGB LEDController::status()
{
    Application::log("Status Called");
    return colour;
}

bool LEDController::toggle()
{
    return true;
}

bool LEDController::toggle(bool state)
{
    return true;
}

bool LEDController::update()
{
    if(state)
    {

    }
    else
    {

    }
    return true;
}

 LEDController::LEDController()
{
    //gpioTerminate();
    //gpioInitialise();

    /*
    gpioSetMode(18, PI_OUTPUT);
    gpioSetMode(19, PI_OUTPUT);
    gpioSetMode(20, PI_OUTPUT);
    */

    // Turn the lights on
    this->state = true;
    //this->update();
    //return true;
    Application::log("Ready");
}



