#ifndef TALKER_H
#define TALKER_H

#include <QObject>
#include <QTextToSpeech>
#include <QVoice>

class Talker : public QObject
{
    Q_OBJECT
public:
    explicit Talker(QObject *parent = nullptr);

    void setRate(double rate);
    void setPitch(double pitch);
    void talk(const QString &text);

    void talkObstacle();
    void talkWelcome();
    void talkStarted();
    void talkStopped();
    void talkBatteryLow();
    void talkOverCurrent();
    void talkTurningError();
    void toggleIsDirty();
    void setIsDirty(bool isDirty);

    double getRate() const;
    double getPitch() const;

    bool getEnabled() const;
    void setEnabled(bool enabled);

signals:

public slots:

private:

    bool _enabled;
    QTextToSpeech *speech;
    bool _isDirty = false; //from settings file
    bool setVoice(); //return 0 for error.
};

#endif // TALKER_H

