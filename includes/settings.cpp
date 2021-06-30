#include "settings.h"
#include <QDebug>
#include <QFileInfo>
#include <QThread>

Settings::Settings(QObject *parent) : QObject(parent)
{
    initFilWatcher();
    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged()));
    getAllParams();
    qDebug() << "Settings::Settings watcher.files():" << watcher.files();
}

void Settings::onFileChanged()
{
    qDebug() << "settings file changed";
    QThread::msleep(10);//the file seems to pop temporarily out of existence.

    if(getAllParams())
    {
        emit settingsChanged();
    }

    QThread::msleep(10);//the file seems to pop temporarily out of existence.
    //re-add the file to watcher
    initFilWatcher();
    qDebug() << "watcher.files():" << watcher.files();
}


//////// Init file Watcher //////////

bool Settings::initFilWatcher()
{
   if(!QFileInfo::exists(settingsPath))
   {
       qDebug() << "Settings::initFilWatcher() << settings file not found!";
       return false;
   }

   if(!watcher.addPath(settingsPath))
   {
       qDebug() << "Settings::initFilWatcher() << watcher.addpath() failed!";
       return false;
   }

   return true;
}

bool Settings::getAllParams()
{
    if(!QFileInfo::exists(settingsPath))
    {
        qDebug() << "Settings::getAllParams() << file not found!";
        return false;
    }

    QSettings qsettings(settingsPath, QSettings::IniFormat);

    qsettings.beginGroup("Motor Settings");
    motorSettings.dutyCycle = qsettings.value("dutyCycle").toInt();
    motorSettings.surgeDC = qsettings.value("surgeDC").toInt();
    motorSettings.surgeDelay = qsettings.value("surgeDelay").toInt();
    motorSettings.frequency = qsettings.value("frequency").toInt();
    motorSettings.turningDC = qsettings.value("turningDC").toInt();
    motorSettings.turningMS = qsettings.value("turningMS").toInt();
    motorSettings.switchDelay = qsettings.value("switchDelay").toInt();
    motorSettings.backupDuration = qsettings.value("backupDuration").toInt();
    qsettings.endGroup();

    qsettings.beginGroup("Voice Settings");
    voiceSettings.pitch = qsettings.value("pitch").toDouble();
    voiceSettings.rate = qsettings.value("rate").toDouble();
    voiceSettings.isDirty = qsettings.value("isDirty").toBool();
    voiceSettings.enabled = qsettings.value("enabled").toBool();
    qsettings.endGroup();

    qsettings.beginGroup("OI Settings");
    oiSettings.OiNums = qsettings.value("OINums").toInt();
    oiSettings.OiPollInterval = qsettings.value("OiPollInterval").toInt();
    qsettings.endGroup();

    qsettings.beginGroup("Prox Settings");
    proxSettings.enabled = qsettings.value("proxEnabled").toBool();

    return true;
}

bool Settings::getTalkEnabled()
{
    QSettings qsettings(settingsPath, QSettings::IniFormat);
    qsettings.beginGroup("Voice Settings");
    voiceSettings.enabled = qsettings.value("enabled").toBool();
    return voiceSettings.enabled;
}







//bool Settings::initFilWatcher() //this shit is buggy on linux...?
//{
//    bool success = false;
//    int retries = 10;
//    int i = 0;

//    while(!success && i < retries)
//    {
//        success = QFileInfo::exists(settingsPath);
//        if(!success)
//        {
//            qDebug() << "settings file not found! attempt: " << i;
//        }
//        i++;
//        QThread::msleep(10);
//    }

//    if(!success)
//    {
//        return false;
//    }

//    //add path to watcher
//    success = false;
//    i = 0;

//    while(!success && i < retries)
//    {
//        success = watcher.addPath(settingsPath);
//        if(!success)
//        {
//            qDebug() << "addpath() failed attempt " << i;
//        }
//        i++;
//    }

//    return success;
//}






