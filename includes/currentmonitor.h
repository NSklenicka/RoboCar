#ifndef CURRENTMONITOR_H
#define CURRENTMONITOR_H

#include <QObject>
#include <Pins.h>
#include <QTimer>

class CurrentMonitor : public QObject
{
    Q_OBJECT
public:
    explicit CurrentMonitor(QObject *parent = nullptr);

    void start();
    void startWhileTurning();
    void stop();

    bool getIsOverCurrent();

    void setPollingInterval(int interval_ms);
    void setNumPollsToTrigger(int polls);

    int getPollingInterval() const;
    int getNumPollsToTrigger() const;

    void setStartDelayMS(int startDelayMS);

signals:

    void overCurrentDetected();
    void oiWhileTurning();

private:

    bool overCurrentHolds();
    int _startDelayMS; // from settings file
    QTimer *timer;
    int _pollingInterval; //from settings file
    int _numPollsToTrigger; //from settings file
    bool _turning;

public slots:

    void pollForOverCurrent();

};

#endif // CURRENTMONITOR_H
