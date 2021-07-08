#include "currentmonitor.h"

CurrentMonitor::CurrentMonitor(QObject *parent) : QObject(parent)
{
    LOG
    WiringPi::pinMode(PIN_OVERCURRENT_SENSE, INPUT);
    WiringPi::pullUpDnControl(PIN_OVERCURRENT_SENSE, PUD_DOWN);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(pollForOverCurrent()));
}

void CurrentMonitor::start()
{
    LOG
    _turning = false;
    DELAY(_startDelayMS);
    timer->start(_pollingInterval);
}

void CurrentMonitor::startWhileTurning()
{
    LOG
    _turning = true;
    DELAY(_startDelayMS);
    timer->start(_pollingInterval);
}

void CurrentMonitor::stop()
{
    LOG
    timer->stop();
    timer->start(_pollingInterval);
}

bool CurrentMonitor::overCurrentHolds()
{
    LOG
    DELAY(_pollingInterval);
    int i = 0;
    while(getIsOverCurrent()) // while OI holds true
    {
        DELAY(_pollingInterval);
        i = i+1;
        if(i == _numPollsToTrigger)
        {
            return true;
        }
    }

    qDebug() << "OI did not hold. i: " << i;
    return false;
}

void CurrentMonitor::setStartDelayMS(int newStartDelayMS)
{
    _startDelayMS = newStartDelayMS;
}

void CurrentMonitor::pollForOverCurrent()
{
    //LOG
    if(getIsOverCurrent())
    {
        timer->stop();
        //if OI holds, then emit
        if(overCurrentHolds())
        {
            qDebug() << " emit overCurrentDetected()";
            if(_turning)
            {
                emit oiWhileTurning();
            }
            else
                emit overCurrentDetected();
        }
        else //resume normal polling
        {
            timer->start(_pollingInterval);
        }
    }
}

bool CurrentMonitor::getIsOverCurrent()
{
    int read = WiringPi::digitalRead(PIN_OVERCURRENT_SENSE);

    if(1 == read)
    {
        return true;
    }
    else return false;
}

/******************************************************* Getters / Setters ***********************************************************/

int CurrentMonitor::getPollingInterval() const
{
    return _pollingInterval;
}

int CurrentMonitor::getNumPollsToTrigger() const
{
    return _numPollsToTrigger;
}

void CurrentMonitor::setPollingInterval(int interval_ms)
{
    LOG
    _pollingInterval = interval_ms;
    if(timer->isActive())
        timer->start(interval_ms);
}

void CurrentMonitor::setNumPollsToTrigger(int pollsToTrigger)
{
    _numPollsToTrigger = pollsToTrigger;
}
