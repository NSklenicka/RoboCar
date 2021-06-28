#include "soundsensor.h"

SoundSensor::SoundSensor(QObject *parent) : QObject(parent)
{
    LOG
    WiringPi::pinMode(PIN_SOUND_SENSOR, INPUT);
    WiringPi::pinMode(PIN_SOUND_RESET, OUTPUT);
    WiringPi::pullUpDnControl(PIN_SOUND_SENSOR, PUD_DOWN);

    pollingTimer = new QTimer(this);
    connect(pollingTimer, &QTimer::timeout, this, &SoundSensor::pollForSound);
    reset();
}

void SoundSensor::reset()
{
    LOG
    //DELAY(20); // so I can see the LED turn on....
    WiringPi::digitalWrite(PIN_SOUND_RESET, 0);
    DELAY(10);
    WiringPi::digitalWrite(PIN_SOUND_RESET, 1);
}

void SoundSensor::setPollingInterval(int pollingInterval)
{
    _pollingInterval = pollingInterval;
    if(pollingTimer->isActive())
        pollingTimer->start(pollingInterval);
}

void SoundSensor::startPollingSingleTrigger()
{
    LOG
    _singleTrigger = true;
    pollingTimer->start(_pollingInterval);
}

void SoundSensor::startPollingMultiTrigger()
{
    LOG
    _singleTrigger = false;
    pollingTimer->start(_pollingInterval);
}

//void SoundSensor::start(bool singleTrigger)
//{
//    LOG
//    _singleTrigger = singleTrigger;
//    pollingTimer->start(_pollingInterval);
//}

void SoundSensor::stop()
{
    pollingTimer->stop();
}

bool SoundSensor::getLatchState()
{
    //LOG
    if(1 == WiringPi::digitalRead(PIN_SOUND_SENSOR)) //if latch is triggered
    {
        reset(); //reset the latch
        return true;
    }
    return false;
}

void SoundSensor::waitForMultipleTriggers()
{
    LOG
    int timeoutMS = 800;
    int triggers = 1; //default to 1 trigger if no other sound detected
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(timeoutMS);

    while(timer.remainingTime() > 0) //if we go 1 second with no other sound detected, break from loop
    {
        if(getLatchState()) //if sound detected
        {
            triggers++;
            timer.start(timeoutMS); //keep the timer alive
        }
        DELAY(50);
    }

    qDebug() << QString("emit soundDetected(%1)").arg(triggers);
    emit soundDetected(triggers);
    //sound sensor must now be re-started by handler
}

void SoundSensor::pollForSound()
{
    //LOG
    if(getLatchState())  //if latch is high
    {
        //stop polling
        pollingTimer->stop();

        if(_singleTrigger)
        {
            qDebug() << "emit soundDetected(1)";
            emit soundDetected(1);
        }
        else
        {
            waitForMultipleTriggers();
        }
    }
    //sound sensor must now be re-started by handler
}
