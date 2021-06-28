#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <Pins.h>

class SoundSensor : public QObject
{
    Q_OBJECT
public:
    explicit SoundSensor(QObject *parent = nullptr);

    void setPollingInterval(int pollingInterval);

    /* void SoundSensor::start(bool singleTrigger = false)
     * if single trigger, code will emit soundDetected(1) on the first sound it detects.
     * if not single trigger, it will emit the number of consecutive sounds it detects within a time frame
     * For example, if you clap 3 times, and !singleTrigger, it will emit soundDetected(3)
     * if you clap 3 times, and singleTrigger, it will emit soundDetected(1) 3 times
     */

    //void start(bool singleTrigger = false);
    void startPollingSingleTrigger();
    void startPollingMultiTrigger();

    void stop(); //stop polling the latch

    /* bool SoundSensor::getLatchState()
     * read the latch state.
     * return true if sound sensor has been triggered (if latch output is high)
     * also resets the latch
     */
    bool getLatchState();

signals:

    void soundDetected(int triggers);

private:

    QTimer *pollingTimer;

    int _pollingInterval = 250;

    bool _singleTrigger;

    void waitForMultipleTriggers();

    void reset(); //set the latch back to low output

public slots:

    void pollForSound();
};

#endif // SOUNDSENSOR_H
