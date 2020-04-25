#include "LEDController.h"

bool LEDController::check()
{
    return true;
}

RGB LEDController::status()
{
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
