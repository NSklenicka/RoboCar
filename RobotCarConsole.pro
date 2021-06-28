QT -= gui
QT += texttospeech

CONFIG += c++11 console
CONFIG -= app_bundle

LIBS += -lwiringPi

INCLUDEPATH += /home/pi/Desktop/dev/RobotCarConsole/includes

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += CONSOLE

SOURCES += \
        main.cpp \
    includes/currentmonitor.cpp \
    includes/dualmotorcontroller.cpp \
    includes/indicatorled.cpp \
    includes/opticproxsensor.cpp \
    includes/powermonitor.cpp \
    includes/robotcar.cpp \
    includes/settings.cpp \
    includes/soundsensor.cpp \
    includes/talker.cpp

DISTFILES += \
    includes/settings.txt

HEADERS += \
    includes/currentmonitor.h \
    includes/dualmotorcontroller.h \
    includes/indicatorled.h \
    includes/opticproxsensor.h \
    includes/Pins.h \
    includes/powermonitor.h \
    includes/robotcar.h \
    includes/settings.h \
    includes/soundsensor.h \
    includes/talker.h


