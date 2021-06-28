#ifndef INDICATORLED_H
#define INDICATORLED_H

#include <QObject>
#include <Pins.h>

class IndicatorLED : public QObject
{
    Q_OBJECT
public:
    explicit IndicatorLED( int pin, QObject *parent = nullptr );

    void turnOn();
    void turnOff();


    //void blink(int times = 0);

    int getBlinkDelay() const;
    void setBlinkDelay(int blinkDelay);

private:

    int blinkDelay = 800;
    int _pin;

signals:

public slots:
};

#endif // INDICATORLED_H
