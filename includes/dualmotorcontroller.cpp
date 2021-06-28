#include "dualmotorcontroller.h"
#include <QProcess>

#ifdef CONSOLE
    #include <QCoreApplication>
    #define PROCESS_EVENTS QCoreApplication::processEvents()
#else
    #include <QApplication>
    #define PROCESS_EVENTS QApplication::processEvents()
#endif

/************************   Constructor  ***************************/

DualMotorController::DualMotorController(QObject *parent) : QObject(parent)
{
    LOG
    WiringPi::pinMode(PIN_MOTOR_ENABLE, OUTPUT);
    //qDebug()<< "testing";
    //setFrequency(_frequency); //settings frequency in RobotCar::setAllParams()
}

/************************   Write to Pins ***************************/
//   Both motors working together

void DualMotorController::writeToPwmPins(int dutyCycleLeft, int dutyCycleRight)
{
    LOG
    if(0 == _direction)
    {
        WiringPi::pinMode(_pin1, PWM_OUTPUT);
        WiringPi::pinMode(_pin2, OUTPUT);
        WiringPi::pinMode(_pin3, PWM_OUTPUT);
        WiringPi::pinMode(_pin4, OUTPUT);

        WiringPi::pwmWrite(_pin1, dutyCycleLeft);
        WiringPi::digitalWrite(_pin2, 0);
        WiringPi::pwmWrite(_pin3, dutyCycleRight);
        WiringPi::digitalWrite(_pin4, 0);
    }
    else
    {
        WiringPi::pinMode(_pin1, OUTPUT);
        WiringPi::pinMode(_pin2, PWM_OUTPUT);
        WiringPi::pinMode(_pin3, OUTPUT);
        WiringPi::pinMode(_pin4, PWM_OUTPUT);

        WiringPi::digitalWrite(_pin1, 0);
        WiringPi::pwmWrite(_pin2, dutyCycleLeft);
        WiringPi::digitalWrite(_pin3, 0);
        WiringPi::pwmWrite(_pin4, dutyCycleRight);
    }

    WiringPi::pwmSetRange(_range);
    WiringPi::pwmSetMode(PWM_MODE_MS);
    WiringPi::pwmSetClock(_clk);
}

/************************   Backup Turn and Resume  ***************************/

void DualMotorController::backupTurnAndResume()
{
    stop();

    DELAY(500);

    //backup slightly
    invertDirection();
    backup(); //move straight in _direction for _backupDuration mSecs

    //adding this because it seems to just follow the same path
    //in reverse when it gets stuck
    invertDirection();

    //rotate for a period of time
    rotate();
    DELAY(_msToTurnFor);
    setEnable(false);

    //resume
    start();
}

/************************   Backup  ***************************/

void DualMotorController::backup()
{
    start();
    DELAY(_backupDuration);
    setEnable(false);
}

/************************   Start For  ***************************/

void DualMotorController::startFor(int ms)
{
    start();
    DELAY(ms);
    stop();
}

/*
void DualMotorController::startForDuration()
{
    start();
    DELAY(_duration);
    stop();
}

*/

/************************  Write Rotate  ***************************/

void DualMotorController::writeRotateToPwmPins(int dutyCycle, bool direction)
{
    LOG
    if(0 == direction)
    {
        WiringPi::pinMode(_pin1, PWM_OUTPUT);
        WiringPi::pinMode(_pin2, OUTPUT);
        WiringPi::pinMode(_pin3, OUTPUT);
        WiringPi::pinMode(_pin4, PWM_OUTPUT);

        WiringPi::pwmWrite(_pin1, dutyCycle);
        WiringPi::digitalWrite(_pin2, 0);
        WiringPi::digitalWrite(_pin3, 0);
        WiringPi::pwmWrite(_pin4, dutyCycle);
    }
    else
    {
        WiringPi::pinMode(_pin1, OUTPUT);
        WiringPi::pinMode(_pin2, PWM_OUTPUT);
        WiringPi::pinMode(_pin3, PWM_OUTPUT);
        WiringPi::pinMode(_pin4, OUTPUT);

        WiringPi::digitalWrite(_pin1, 0);
        WiringPi::pwmWrite(_pin2, _turningDutyCycle);
        WiringPi::pwmWrite(_pin3, _turningDutyCycle);
        WiringPi::digitalWrite(_pin4, 0);
    }

    WiringPi::pwmSetRange(_range);
    WiringPi::pwmSetMode(PWM_MODE_MS);
    WiringPi::pwmSetClock(_clk);
}

/************************   Stop  ***************************/

void DualMotorController::stop()
{
    LOG
    _isRunning = false;
    setEnable(false);
}

/************************   Start  ***************************/

void DualMotorController::start()
{
    LOG
    _isRunning = true;
    surgeAndDelay();
    writeToPwmPins(_dutyCycle, _dutyCycle);
}

/************************   Surge and Delay  ***************************/

void DualMotorController::surgeAndDelay()
{
    LOG
    if ( _surgeDutyCycle > 0 )
        writeToPwmPins(_surgeDutyCycle, _surgeDutyCycle);
    setEnable(true);
    DELAY(_surgeDelay);
}

/************************   Rotate  ***************************/

void DualMotorController::rotate()
{
    //_isRunning = true; //should already be set to true by backup()
    //rotate
    writeRotateToPwmPins(_turningDutyCycle, _direction);
    setEnable(true);
}

/************************   Set Enable  ***************************/

void DualMotorController::setEnable(bool enable)
{
    LOG
    _isEnabled = enable;
    WiringPi::digitalWrite(PIN_MOTOR_ENABLE, enable);
}

/************************   Get Pi Model   ***************************/

QString DualMotorController::getPiModel()
{
    LOG
    QProcess process;
    process.setProgram("cat");
    QStringList args("/proc/device-tree/model");
    process.setArguments(args);
    process.start();

    while(!process.waitForReadyRead())
        PROCESS_EVENTS;

    QString output = process.readAllStandardOutput();
    process.close();
    return output;
}

/************************   Invert Direction  ***************************/

void DualMotorController::invertDirection()
{
    LOG
    bool otherDirection = !_direction;
    _direction = otherDirection;
}

/************************   Switch Direction  ***************************/

void DualMotorController::switchDirection()
{
    invertDirection();
    if(_isRunning)
    {
        stop();
        DELAY(_switchDelay);
        start();
    }
}

/************************   Set Frequency  ***************************/

void DualMotorController::setFrequency(int frequency)
{
    LOG
    if(frequency > 100000 || frequency < 0)
    {
        qDebug() << "Error: Frequency is out of range!";
        return;
    }
	
	if(frequency == _frequency)
		return;

    _frequency = frequency;

    //calculate and set the clock divider
    int k = getFrequencyConstant();
    qDebug() << "k set to " << k;
    _clk = k / (_range * _frequency);
}

/********************   Get Frequency Constant     ********************************/

int DualMotorController::getFrequencyConstant()
{
    LOG
    QString model4B_1p2 = "Raspberry Pi 4 Model B Rev 1.2";
    QString model4_1p1 = "Raspberry Pi 4 Model B Rev 1.1";
    QString model2B = "Raspberry Pi 2 Model B Rev 1.1";
    QString modelZeroW = "Raspberry Pi Zero W Rev 1.1";

    QString model = getPiModel();

    if(model == model4_1p1)
    {
        return (54000000);
        //return (54000000*2);
    }
    if(model == model4B_1p2)
    {
        return 54000000;
    }
    else if (model == model2B)
    {
        return 19200000;
    }
    else if (model == modelZeroW)
    {
        return 19200000;
    }
    else
    {
        qDebug() << QString("model [%1] not registered!").arg(model);
        return -1;
    }
}

/************************   Turn  ***************************/

//void DualMotorController::turn(bool rightLeft)
//{
//    LOG
//            _isRunning = true;
//    setEnable(false);
//    if(rightLeft)
//    {
//        writeToPwmPins(_dutyCycle, _turningDutyCycle);
//    }
//    else
//    {
//        writeToPwmPins(_turningDutyCycle, _dutyCycle);
//    }
//    setEnable(true);
//}


/************************   Getters & Setters    ***************************/


int DualMotorController::getFrequency() const
{
    return _frequency;
}

int DualMotorController::getSwitchDelay() const
{
    return _switchDelay;
}

void DualMotorController::setSwitchDelay(int switchDelay)
{
    _switchDelay = switchDelay;
}

int DualMotorController::getMsToTurnFor() const
{
    return _msToTurnFor;
}

bool DualMotorController::getDirection() const
{
    return _direction;
}

bool DualMotorController::getIsRunning() const
{
    return _isRunning;
}

void DualMotorController::setMsToTurnFor(int msToTurnFor)
{
    _msToTurnFor = msToTurnFor;
}

int DualMotorController::getDutyCycle() const
{
    return _dutyCycle;
}

void DualMotorController::setDutyCycle(int dutyCycle)
{
    LOG
    if(dutyCycle > _range || dutyCycle < 0)
    {
        qDebug() << "Error: Duty cycle is out of range!";
        return;
    }

    _dutyCycle = dutyCycle;
}

int DualMotorController::getTurningDutyCycle() const
{
    return _turningDutyCycle;
}

void DualMotorController::setTurningDutyCycle(int turningDutyCycle)
{
    LOG
    if(turningDutyCycle > _range || turningDutyCycle < 0 || turningDutyCycle > _dutyCycle)
    {
        qDebug() << "Error: Turning duty cycle is out of range!";
        return;
    }

    _turningDutyCycle = turningDutyCycle;
}

int DualMotorController::getSurgeDelay() const
{
    return _surgeDelay;
}

void DualMotorController::setSurgeDelay(int surgeDelay)
{
    LOG
    if(surgeDelay < 0)
    {
        qDebug() << "Error: surge delay is out of range!";
        return;
    }

    _surgeDelay = surgeDelay;
}

int DualMotorController::getSurgeDutyCycle() const
{
    return _surgeDutyCycle;
}

void DualMotorController::setSurgeDutyCycle(int surgeDutyCycle)
{
    LOG
    if(surgeDutyCycle > _range || surgeDutyCycle < 0)
    {
        qDebug() << "Error: Surge duty cycle is out of range!";
        return;
    }

    _surgeDutyCycle = surgeDutyCycle;
}

int DualMotorController::getDuration() const
{
    return _duration;
}

void DualMotorController::setDuration(int duration)
{
    _duration = duration;
}

int DualMotorController::getBackupDuration() const
{
    return _backupDuration;
}

void DualMotorController::setBackupDuration(int backupDuration)
{
    _backupDuration = backupDuration;
}

void DualMotorController::setDirection(bool direction)
{
	_direction = direction;
}


