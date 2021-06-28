#ifndef OPTICPROXSENSOR_H
#define OPTICPROXSENSOR_H

#include <QObject>
#include <QTimer>
#include <Pins.h>

class OpticProxSensor : public QObject
{
    Q_OBJECT
public:

    explicit OpticProxSensor( int piPin, QObject *parent = nullptr);

    void start();
    void stop();
    void setPollingInterval(int interval);
    bool getIsObstructed();
    void setEnabled(bool enabled);

signals:

    void obstructionDetected();

private:

    QTimer *timerPoll;
    int _pollingInterval = 150;
    int _pin;
    bool _enabled;

    void pollForObstruction();

};

#endif // OPTICPROXSENSOR_H
