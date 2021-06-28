#ifndef DUALMOTORCONTROLLER_H
#define DUALMOTORCONTROLLER_H

#include <QObject>
#include <Pins.h>

class DualMotorController : public QObject
{
    Q_OBJECT
public:

    explicit DualMotorController(QObject *parent = nullptr);

    void stop(); // Sets enable line to low. Stops motor driver.

    void start(); // Move in _direction

    void backup(); // move straight in _direction for _backupDuration mSecs

    void setEnable(bool enable); // Set the enable line on the L293 motor driver

    void invertDirection(); // inverts current direction.

    void switchDirection(); //invert then start

    void surgeAndDelay(); // does nothing when surge dc or surge delay is set to 0

    //void turn( bool rightLeft = 1 );

    void rotate();

    void backupTurnAndResume();
	
	void startFor(int ms);

    //void startForDuration();

    void writeRotateToPwmPins(int dutyCycle = 100, bool direction = 0 );

    QString getPiModel();

    void setFrequency(int frequency);

    int getFrequencyConstant();


    // Getters & Setters

    bool getIsRunning() const;
    bool getDirection() const;
	void setDirection(bool direction);
    int getDutyCycle() const;
    void setDutyCycle(int dutyCycle);
    int getTurningDutyCycle() const;
    void setTurningDutyCycle(int turningDutyCycle);
    int getSurgeDelay() const;
    void setSurgeDelay(int surgeDelay);
    int getSurgeDutyCycle() const;
    void setSurgeDutyCycle(int surgeDutyCycle);
    void setMsToTurnFor(int msToTurnFor);
    int getMsToTurnFor() const;
    int getSwitchDelay() const;
    void setSwitchDelay(int switchDelay);
    int getFrequency() const;
    int getDuration() const;
    void setDuration(int duration);
    int getBackupDuration() const;
    void setBackupDuration(int backupDuration);

private:

    void writeToPwmPins( int dutyCycleLeft, int dutyCycleRight );

    const int _range = 100;
    bool _direction = 0;
    int _dutyCycle; //from settings file
    int _turningDutyCycle; //from settings file
    int _clk;
    int _frequency; //from settings file
    int _surgeDelay; //from settings file
    int _surgeDutyCycle; //from settings file
    bool _isRunning;
    bool _isEnabled;
    int _msToTurnFor; //from settings file
    int _switchDelay = 700;
    int _duration = 0;
    int _backupDuration; //from settings file

    int _pin1 = PIN_MOTOR_1;
    int _pin2 = PIN_MOTOR_2;
    int _pin3 = PIN_MOTOR_3;
    int _pin4 = PIN_MOTOR_4;

signals:

public slots:
};

#endif // DUALMOTORCONTROLLER_H
