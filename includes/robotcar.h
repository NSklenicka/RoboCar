#ifndef ROBOTCAR_H
#define ROBOTCAR_H

#include <QObject>
#include <opticproxsensor.h>
#include <Pins.h>
#include <soundsensor.h>
#include <currentmonitor.h>
#include <dualmotorcontroller.h>
#include <talker.h>
#include <indicatorled.h>
#include <powermonitor.h>
#include <settings.h>

class RobotCar : public QObject
{
    Q_OBJECT
public:
    explicit RobotCar(QObject *parent = nullptr);
    ~RobotCar();

    DualMotorController motor;
    OpticProxSensor *proxSensorFront;
    OpticProxSensor *proxSensorRear;
    SoundSensor *soundSensor;
    CurrentMonitor *currentMonitor;
    Talker *talker;
    IndicatorLED *greenLED;
    IndicatorLED *redLED;
    PowerMonitor *powerSupplyMonitor;
    Settings *settings;

    void setAllParams();

//    void startRobotCar();
//    void stopRobotCar();

signals:

private:

    void handleObstruction();
    void processArgs();

public slots:

    void onObstructionFrontDetected();
    void onObstructionRearDetected();
    void onSoundDetected(int triggers);
    void onOverCurrentDetected();
    void onLowVoltageDetected();
    void onOiWhileTurning();
    void onSettingsChanged();
    void onCalOI();
    void onCalOITurning();
};

#endif // ROBOTCAR_H
