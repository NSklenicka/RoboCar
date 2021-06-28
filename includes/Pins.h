#include <QTime>
//#include <chrono>
//#include <thread>
#include <QDebug>
#include <QThread>

namespace WiringPi
{
    #include <wiringPi.h>
}

#define PIN_PROX_FRONT      29  //physical 29 //Active Low //wiringPi 21
#define PIN_PROX_REAR       31  //physical 31 //Active Low //wiringPi 22

#define PIN_SOUND_SENSOR    8   //physical 8 //Active high //wiringPi 15
#define PIN_SOUND_RESET     10  //physical 10 //Active low (low = reset) //wiringPi 16

#define PIN_MOTOR_ENABLE    36  //physical 36 // Active high
#define PIN_MOTOR_1         12  //physical 12 // PWM0
#define PIN_MOTOR_2         32  //physical 32 // PWM0
#define PIN_MOTOR_3         33  //physical 33 // PWM1
#define PIN_MOTOR_4         35  //physical 35 // PWM1

#define PIN_OVERCURRENT_SENSE 40 //physical pin 40 //wiringPi 29

#define PIN_LED_GREEN       3   //physical 3 // wiringPi 8
#define PIN_LED_RED         5   //physical 5 // wiringPi 9
#define LED_ACTIVE          1   //LED active logic state
#define LED_INACTIVE        0   //LED off logic state

#define LOG qDebug() << __PRETTY_FUNCTION__;
//#define DELAY(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#define DELAY(ms) QThread::msleep(ms);

//#define PIN_PWM_FORWARD 33  //physical 33 //wiringPi 23
//#define PIN_PWM_BACKWARD 32 //physical 32 //wiringPi 26
//no physical 27 or 28
