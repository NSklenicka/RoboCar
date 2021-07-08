#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QSettings>

struct MotorSettings
{
    int dutyCycle;
    int surgeDC;
    int surgeDelay;
    int frequency ;
    int turningDC;
    int turningMS;
    int switchDelay;
    int backupDuration;
};

struct VoiceSettings
{
    double pitch;
    double rate;
    bool isDirty;
    bool enabled;
};

struct OISettings
{
    int OiPollInterval;
    int OiNums;
};

struct ProxSettings
{
    bool enabled;
};

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    bool getAllParams(); // read all settings from file, store in structs
    bool getTalkEnabled();

    MotorSettings motorSettings;
    VoiceSettings voiceSettings;
    OISettings oiSettings;
    ProxSettings proxSettings;

signals:
    void settingsChanged();

public slots:
    void onFileChanged();

private:
    QString settingsPath = "/home/pi/Desktop/dev/robocar/includes/settings.txt";
    QFileSystemWatcher watcher;
    bool initFilWatcher();

};

#endif // SETTINGS_H
