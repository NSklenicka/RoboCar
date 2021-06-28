#include "talker.h"
#include <QDebug>
#include <QRandomGenerator>
#include <Pins.h>

Talker::Talker(QObject *parent) : QObject(parent)
{
    LOG
    speech = new QTextToSpeech("speechd", this);
    auto availableEngines = speech->availableEngines();
    qDebug() << "Engines: "<< availableEngines;
    setVoice();
    qDebug() << "voice: " << speech->voice().name();
    //setRate(_rate);
    //setPitch(_pitch);
    speech->setVolume(100);
}

bool Talker::setVoice()
{
    LOG
    speech->setLocale(QLocale(QLocale::English, QLocale::LatinScript, QLocale::UnitedStates));
    QVector <QVoice> availableVoices = speech->availableVoices();

    if(availableVoices.size() < 18)
    {
        qDebug() << "Error: availableVoices.size() < 18. availableVoices.size() = " << availableVoices.size();
        speech->setVoice(availableVoices.at(0));
        return 0;
    }
    else
    {
        speech->setVoice(availableVoices.at(17));
        return 1;
    }
}

void Talker::setRate(double rate)
{
    LOG
    if(rate > 1 || rate < -1)
    {
        qDebug() << "Error: speech rate out of range";
        return;
    }
    speech->setRate(rate);
    qDebug() << "speech->rate(): " << speech->rate();
}

void Talker::setPitch(double pitch)
{
    LOG
    if(pitch > 1 || pitch < -1)
    {
        qDebug() << "Error: speech pitch out of range";
        return;
    }
    speech->setRate(pitch);
    qDebug() << "speech->pitch(): " << speech->pitch();
}

void Talker::talk(const QString &text)
{
    LOG
    //speech->stop();
    speech->say(text);
}

void Talker::talkObstacle()
{
    if(!_enabled)
        return;
    LOG
    QStringList list;

    if(_isDirty)
    {
        list << "god dammit";
        list << "fuck";
    }
    else
    {
        list << "hey";
        list << "oops";
        list << "who put that there";
        list << "you cant fool me";
    }

    int random = random = QRandomGenerator::global()->bounded(list.size());
    QString str = list.at(random);
    speech->stop();
    speech->say(str);
}

void Talker::talkWelcome()
{
    if(!_enabled)
        return;
    LOG
    QStringList list;

    if(_isDirty)
    {
        list << "what the fuck am I doing here";
        list << "I can't believe you woke me up for this shit";
    }
    else
    {
        list << "What is my purpose";
        list << "watch out I am a crazy driver";
    }

    int random = random = QRandomGenerator::global()->bounded(list.size());
    QString str = list.at(random);
    speech->stop();
    speech->say(str);
}

void Talker::talkStarted()
{
    if(!_enabled)
        return;
    LOG
    QStringList list;

    if(_isDirty)
    {
        list << "fuck this shit";
        list << "I only had 1 drink I swear";
    }
    else
    {
        list << "jeez that was loud";
        list << "vroom vroom";
        list << "alright";
    }

    int random = random = QRandomGenerator::global()->bounded(list.size());
    QString str = list.at(random);
    speech->stop();
    speech->say(str);
}

void Talker::talkStopped()
{
    if(!_enabled)
        return;
    LOG
    QStringList list;

    if(_isDirty)
    {
        list << "man I needed a fucking brake anyway";
    }
    else
    {
        list << "Man I was just gettings warmed up";
        list << "brake check";
        list << "make up your mind";
    }

    int random = random = QRandomGenerator::global()->bounded(list.size());
    QString str = list.at(random);
    speech->stop();
    speech->say(str);
}

void Talker::talkBatteryLow()
{
    if(!_enabled)
        return;
    LOG
    QStringList list;

    if(_isDirty)
    {
        list << "shit man I am about to die";
    }
    else
    {
        list << "the battery is running low. I will now shut down.";
    }

    int random = random = QRandomGenerator::global()->bounded(list.size());
    QString str = list.at(random);
    speech->stop();
    speech->say(str);
}

void Talker::talkOverCurrent()
{
    if(!_enabled)
        return;
    LOG
    QStringList list;

    if(_isDirty)
    {
        list << "How the fuck did I not see that";
        list << "shit";
    }
    else
    {
        list << "Oops";
    }

    int random = random = QRandomGenerator::global()->bounded(list.size());
    QString str = list.at(random);
    speech->stop();
    speech->say(str);
}

void Talker::talkTurningError()
{
    if(!_enabled)
        return;
    LOG
    QStringList list;

    if(_isDirty)
    {
        list << "man I am really fucked now";
        list << "shit man I am stuck";
    }
    else
    {
        list << "dude I think I am stuck";
        list << " I need some help";
    }

    int random = random = QRandomGenerator::global()->bounded(list.size());
    QString str = list.at(random);
    speech->stop();
    speech->say(str);
}

void Talker::toggleIsDirty()
{
    if(!_enabled)
        return;
    LOG
    _isDirty = !_isDirty;
    qDebug() << "isDirty: " << _isDirty;

    if(_isDirty)
        speech->say("Dirty mode enabled");
    else
        speech->say("Dirty mode disabled");
}

void Talker::setIsDirty(bool isDirty)
{
    LOG
    _isDirty = isDirty;
}

double Talker::getRate() const
{
    return speech->rate();
}

double Talker::getPitch() const
{
    return speech->pitch();
}

bool Talker::getEnabled() const
{
    return _enabled;
}

void Talker::setEnabled(bool enabled)
{
    _enabled = enabled;
}




