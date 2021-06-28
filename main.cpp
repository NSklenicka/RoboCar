#include <QCoreApplication>
#include <robotcar.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WiringPi::wiringPiSetupPhys();

    RobotCar robotCar;

    return a.exec();
}
