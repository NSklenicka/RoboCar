#ifndef POWERMONITOR_H
#define POWERMONITOR_H

#include <QObject>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <Pins.h>

class PowerMonitor : public QObject
{
    Q_OBJECT
public:
    explicit PowerMonitor(QObject *parent = nullptr);

    void start();
    void stop();
    QString getStats();
    void shutdownPi();
    void setPollingInterval(int pollingInterval_ms);

signals:

    void underVoltageDetected();

private:

    QTimer *timer;
    int _pollingInterval;
    void processOutput(QString &output, bool &powerOK);

public slots:

    //true for ok power state. False for under-voltage detected.
    bool checkSystemPower();
};

#endif // POWERMONITOR_H
