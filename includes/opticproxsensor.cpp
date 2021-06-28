#include "opticproxsensor.h"

OpticProxSensor::OpticProxSensor(int piPin, QObject *parent) : QObject(parent)
{
    LOG
    timerPoll = new QTimer(this);
    connect(timerPoll, &QTimer::timeout, this, &OpticProxSensor::pollForObstruction);

    //wiringPiSetup();
    WiringPi::pinMode(piPin, INPUT);
    WiringPi::pullUpDnControl(piPin, PUD_OFF);
    _pin = piPin;
}

void OpticProxSensor::start()
{
    LOG
    if(_enabled)
        timerPoll->start(_pollingInterval);
    else
        qDebug() << "optic prox sensor not enabled";
}

void OpticProxSensor::stop()
{
    LOG
    timerPoll->stop();
}

void OpticProxSensor::setPollingInterval(int interval)
{
    LOG
    _pollingInterval = interval;
    if(timerPoll->isActive())
        timerPoll->start(interval);
}

bool OpticProxSensor::getIsObstructed()
{
    int read = WiringPi::digitalRead(_pin);
    if(0 == read)
    {
        return true;
    }
    else return false;
}

void OpticProxSensor::pollForObstruction()
{
    //LOG
    if(getIsObstructed())
    {
        timerPoll->stop();
        qDebug() << "Obstacle detected. Timer stopped.";
        emit obstructionDetected();
    }
}

void OpticProxSensor::setEnabled(bool enabled)
{
    _enabled = enabled;
}
