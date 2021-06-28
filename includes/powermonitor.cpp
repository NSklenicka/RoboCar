#include "powermonitor.h"
#include <bitset>

#ifdef CONSOLE
    #include <QCoreApplication>
    #define PROCESS_EVENTS QCoreApplication::processEvents()
#else
    #include <QApplication>
    #define PROCESS_EVENTS QApplication::processEvents()
#endif


PowerMonitor::PowerMonitor(QObject *parent) : QObject(parent)
{
    LOG
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PowerMonitor::checkSystemPower);
    setPollingInterval(30*1000);
    checkSystemPower();
    start();
}

void PowerMonitor::start()
{
    LOG
    timer->start(_pollingInterval);
}

void PowerMonitor::stop()
{
    LOG
    timer->stop();
}

void PowerMonitor::setPollingInterval(int pollingInterval_ms)
{
    _pollingInterval = pollingInterval_ms;
    if(timer->isActive())
        timer->start(pollingInterval_ms);
}

bool PowerMonitor::checkSystemPower()
{
    //LOG
    QString out = getStats();
    bool ok;
    processOutput(out, ok);
    return ok;
}

QString PowerMonitor::getStats()
{
    QProcess process;
    process.setProgram("vcgencmd");
    process.setArguments(QStringList("get_throttled"));
    process.start();

    while(process.waitForReadyRead(5*1000))
    {
        PROCESS_EVENTS;
    }

    QString output = process.readAllStandardOutput();
    //qDebug() << "output: \n" << output;
    return output;
}

void PowerMonitor::shutdownPi()
{
    //"sudo shutdown" will default to one minute
    system("sudo shutdown");
}

void PowerMonitor::processOutput(QString &output, bool &powerOK)
{
    //output =  "throttled=0x0\n";
    if(!output.startsWith("throttled="))
        return;

    output.remove("throttled=");
    output.remove("\n");
    //qDebug() << "output (should just be a string of hex): " << output;

    bool ok;
    int hex = output.toInt(&ok, 16);

    if(!ok)
    {
        qDebug() << "string to int conversion failed!";
        return;
    }

    if(0 == hex)
    {
        //qDebug() << "system power is good";
        powerOK = true;
        return;
    }

    std::bitset<19> bits(hex);

    bool lowVoltage = bits[0];
    if(lowVoltage)
    {
        qDebug() << "Under-voltage detected";
        emit underVoltageDetected();
        powerOK = false;
        return;
    }

    qDebug() << "an error has occurred in " << __PRETTY_FUNCTION__;
}









//    Bit	Hex value	Meaning
//    0     1           Under-voltage detected
//    1     2           Arm frequency capped
//    2     4           Currently throttled
//    3     8           Soft temperature limit active
//    16    10000       Under-voltage has occurred
//    17	20000       Arm frequency capping has occurred
//    18	40000       Throttling has occurred
//    19	80000       Soft temperature limit has occurred

//    19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//     0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1

//    bool wasThrottled = bits[18];
//    if(wasThrottled)
//    {
//        qDebug() << "Throttling has occurred";
//    }

//    bool wasLowVoltage = bits[16];
//    if(wasLowVoltage)
//    {
//       qDebug() << "Under-voltage has occurred";
//    }


