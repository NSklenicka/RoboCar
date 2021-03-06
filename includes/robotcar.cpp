#include "robotcar.h"
#include <QCoreApplication>

RobotCar::RobotCar(QObject *parent) : QObject(parent)
{ 
    motor.stop();

    //LEDs
    greenLED = new IndicatorLED(PIN_LED_GREEN);
    redLED = new IndicatorLED(PIN_LED_RED);

    //power supply monitor
    powerSupplyMonitor = new PowerMonitor;
    connect(powerSupplyMonitor, SIGNAL(underVoltageDetected()), this, SLOT(onLowVoltageDetected()));

    //setup sound sensor
    soundSensor = new SoundSensor;
    connect(soundSensor, SIGNAL(soundDetected(int)), this, SLOT(onSoundDetected(int)));

    //setup motor current monitor
    currentMonitor = new CurrentMonitor;
    connect(currentMonitor, SIGNAL(overCurrentDetected()), this, SLOT(onOverCurrentDetected()));
    connect(currentMonitor, SIGNAL(oiWhileTurning()), this, SLOT(onOiWhileTurning()));
    currentMonitor->start();

    //setup prox sensors
    proxSensorFront = new OpticProxSensor(PIN_PROX_FRONT);
    connect(proxSensorFront, SIGNAL(obstructionDetected()), this, SLOT(onObstructionFrontDetected()));

    proxSensorRear = new OpticProxSensor(PIN_PROX_REAR);
    connect(proxSensorRear, SIGNAL(obstructionDetected()), this, SLOT(onObstructionRearDetected()));

	//settings
    settings = new Settings;
    connect(settings, SIGNAL(settingsChanged()), this, SLOT(onSettingsChanged()));
    setAllParams();

    //setup speech
    ////talker = new //talk;

    QStringList args = QCoreApplication::arguments();
    qDebug() << "args: " << args;
    if(args.count() > 1)
    {
        processArgs();
    }
    else
    {
        proxSensorRear->start();
        proxSensorFront->start();
        //talk->talkWelcome();
        soundSensor->startPollingMultiTrigger();
        greenLED->turnOn();
    }
}

/////////////// DESTRUCTOR ////////////////

RobotCar::~RobotCar()
{
    motor.stop();
}

/////////////// Process Arguments ////////////////

void RobotCar::processArgs()
{
    QStringList args = QCoreApplication::arguments();
    if(args.contains("-cal"))
    {
        calRoutine();
    }
    else if(args.contains("-rotate"))
    {
        rotateRoutine();
    }
    else if(args.contains("stop"))
    {
        motor.stop();
        QCoreApplication::quit();
    }
}

/////////////// Calibration ////////////////

void RobotCar::calRoutine()
{
    qDebug() << "Entering motor calibration routine";
    ////talk->setEnabled(false);
    proxSensorRear->setEnabled(false);
    proxSensorFront->setEnabled(false);
    disconnect(currentMonitor, SIGNAL(overCurrentDetected()), this, SLOT(onOverCurrentDetected()));
    disconnect(currentMonitor, SIGNAL(oiWhileTurning()), this, SLOT(onOiWhileTurning()));
    connect(currentMonitor, SIGNAL(overCurrentDetected()), this, SLOT(onCalOI()));
    motor.start();
    soundSensor->startPollingMultiTrigger();
}

/////////////// Rotate Calibration////////////////

void RobotCar::rotateRoutine()
{
    qDebug() << "Entering rotation calibration routine";
    ////talk->setEnabled(false);
    proxSensorRear->setEnabled(false);
    proxSensorFront->setEnabled(false);
    disconnect(currentMonitor, SIGNAL(overCurrentDetected()), this, SLOT(onOverCurrentDetected()));
    disconnect(currentMonitor, SIGNAL(oiWhileTurning()), this, SLOT(onOiWhileTurning()));
    connect(currentMonitor, SIGNAL(oiWhileTurning()), this, SLOT(onCalOITurning()));
    motor.rotate();
    currentMonitor->startWhileTurning();
    soundSensor->startPollingMultiTrigger();
}

/////////////// Settings ////////////////

void RobotCar::onSettingsChanged()
{
    LOG
    setAllParams();
}

/////////////// Calibration OI detected ////////////////

void RobotCar::onCalOI()
{
    LOG
    motor.switchDirection();
    currentMonitor->start();
}

/////////////// Calibration OI detected while rotating ////////////////

void RobotCar::onCalOITurning()
{
    LOG
    motor.stop();
    motor.invertDirection();
    DELAY(motor.getSwitchDelay());
    motor.rotate();
    currentMonitor->startWhileTurning();
}

/////////////// Set Params ////////////////

void RobotCar::setAllParams()
{
    LOG

    int backupDuration = settings->motorSettings.backupDuration;
    qDebug() << "backupDuration: " << backupDuration;
    motor.setBackupDuration(backupDuration);

    int dc = settings->motorSettings.dutyCycle;
    qDebug() << "dutyCycle: " << dc;
    motor.setDutyCycle(dc);

    int surgeDC = settings->motorSettings.surgeDC;
    qDebug() << "surgeDC: " << surgeDC;
    motor.setSurgeDutyCycle(surgeDC);

    int surgeDelay = settings->motorSettings.surgeDelay;
    qDebug() << "surgeDelay: " << surgeDelay;
    motor.setSurgeDelay(surgeDelay);

    int frequency = settings->motorSettings.frequency;
    qDebug() << "frequency: " << frequency;
    motor.setFrequency(frequency);

    int turningDC = settings->motorSettings.turningDC;
    qDebug() << "turningDC: " << turningDC;
    motor.setTurningDutyCycle(turningDC);

    int turningMS  = settings->motorSettings.turningMS;
    qDebug() << "turningMS: " << turningMS;
    motor.setMsToTurnFor(turningMS);

    int switchDelay = settings->motorSettings.switchDelay;
    qDebug() << "switchDelay: " << switchDelay;
    motor.setSwitchDelay(switchDelay);

    double pitch = settings->voiceSettings.pitch;
    qDebug() << "pitch" << pitch;
    ////talk->setPitch(pitch);

    double rate = settings->voiceSettings.rate;
    qDebug() << "rate: " << rate;
    ////talk->setRate(rate);

    bool isDirty = settings->voiceSettings.isDirty;
    qDebug() << "isDirty: " << isDirty;
    ////talk->setIsDirty(isDirty);

    bool voiceEnabled = settings->voiceSettings.enabled;
    qDebug() << "voiceEnabled: " << voiceEnabled;
    ////talk->setEnabled(voiceEnabled);

    int oiNums = settings->oiSettings.OiNums;
    qDebug() << "oiNums: " << oiNums;
    currentMonitor->setNumPollsToTrigger(oiNums);

    int OiPollInterval = settings->oiSettings.OiPollInterval;
    qDebug() << "OiPollInterval: " << OiPollInterval;
    currentMonitor->setPollingInterval(OiPollInterval);

    bool proxEnabled = settings->proxSettings.enabled;
    qDebug() << "proxEnabled: " << proxEnabled;
    proxSensorRear->setEnabled(proxEnabled);
    proxSensorFront->setEnabled(proxEnabled);
}

/////////////// Sound Detected ////////////////

void RobotCar::onSoundDetected(int triggers)
{
    LOG
    qDebug() << "sounds detected: "<< triggers;
    switch (triggers)
    {
       case 1: //single clap -  toggle motor start
            if(motor.getIsRunning())
            {
                //talk something, but don't stop motor
                soundSensor->startPollingMultiTrigger();
            }
            else //motor is not running
            {
                motor.start();
                //talk->talkStarted();
                soundSensor->startPollingMultiTrigger();
            }
            break;

    case 2:
         //stop
         motor.stop();
         soundSensor->startPollingMultiTrigger();
         break;

    case 3:
        //cal routine
        calRoutine();
        soundSensor->startPollingMultiTrigger();
        break;

    case 4:
          //just spin the car around until interrupted
          motor.rotate();
          soundSensor->startPollingMultiTrigger();
          break;

//    case 5:
//         //toggle dirty spech
//         //talk->toggleIsDirty();
//         soundSensor->startPollingMultiTrigger();
//         break;

       default:
            qDebug() << "Unhandled number of sound triggers: " << triggers;
            //talk->talk("I don't know what you want");
            soundSensor->startPollingMultiTrigger();
            break;
    }
}

/////////////// Over Current Detected ////////////////

void RobotCar::onOverCurrentDetected()
{
    LOG
    //talk->talkOverCurrent();
    handleObstruction();
}

/////////////// Low Supply Voltage Detected ////////////////

void RobotCar::onLowVoltageDetected()
{
    LOG
    motor.stop();
    //talk->talkBatteryLow();
    redLED->turnOn();
    powerSupplyMonitor->shutdownPi();
}

/////////////// OI While Turning ////////////////

void RobotCar::onOiWhileTurning()
{
    LOG
    motor.stop();
    //talk->talkTurningError();
    redLED->turnOn();
}

/////////////// Handle Obstruction ////////////////

void RobotCar::handleObstruction()
{
    LOG
    currentMonitor->startWhileTurning();
    motor.backupTurnAndResume();
    currentMonitor->start();
}

/////////////// Obstructions Detected ////////////////

void RobotCar::onObstructionFrontDetected()
{
    LOG
    if(!motor.getIsRunning()) //car is not moving
    {
        //talk something here
        //move in opposite direction of sensor while sensor is triggered
		motor.setDirection(1);
		motor.start();
		while(proxSensorRear-> getIsObstructed())
        {
            QCoreApplication::processEvents();
        }
		motor.stop();
    }

    //if moving same direction as sensor (forward)
    if(motor.getDirection() == 0)
    {
        //talk->talkObstacle();
        handleObstruction();
    }

    proxSensorFront->start();
}

/////////////////////////////////////////////////////////

void RobotCar::onObstructionRearDetected()
{
	LOG
    if(!motor.getIsRunning()) //car is not moving
    {
		//talk something here
        //move in opposite direction of sensor while sensor is triggered
		motor.setDirection(0);
		motor.start();
		while(proxSensorRear-> getIsObstructed())
            QCoreApplication::processEvents();
		motor.stop();
    }

    //if moving same direction as sensor
    if(motor.getDirection() == 1)
    {
        //talker->talkObstacle();
        handleObstruction();
    }

	//reset sensor
    proxSensorRear->start();
}


/////////////// Not yet implemented.... ////////////////

//void RobotCar::startRobotCar()
//{
//    LOG
//    motor.start();
//    proxSensorRear->start();
//    proxSensorFront->start();
//    currentMonitor->start();
//}

//void RobotCar::stopRobotCar()
//{
//    LOG
//    motor.stop();
//    proxSensorRear->stop();
//    proxSensorFront->stop();
//    currentMonitor->stop();
//}

