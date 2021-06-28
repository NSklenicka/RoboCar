#include "indicatorled.h"

IndicatorLED::IndicatorLED(int pin, QObject *parent)
{
    _pin = pin;
    WiringPi::pinMode(pin, OUTPUT);
}

void IndicatorLED::turnOn()
{
    WiringPi::digitalWrite(_pin, LED_ACTIVE);
}

void IndicatorLED::turnOff()
{
    WiringPi::digitalWrite(_pin, LED_INACTIVE);
}

////turn on, then turn off n times.
//void IndicatorLED::blink(int times)
//{
//    //turn off LED
//    turnOff();

//    if(times > 0) //finite number of times
//    {
//        for(int i = 0; i < times; i++)
//        {

//        }
//    }
//}

int IndicatorLED::getBlinkDelay() const
{
    return blinkDelay;
}

void IndicatorLED::setBlinkDelay(int blinkDelay)
{
    blinkDelay = blinkDelay;
}
