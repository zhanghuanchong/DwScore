#ifndef DWCOUNTDOWN_H
#define DWCOUNTDOWN_H

#include <QtGui/QWidget>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <QLCDNumber>

class DwCountDown : public QWidget
{
    Q_OBJECT

public:
    DwCountDown(QWidget *parent = 0);
    void start();
    void start(int,int,int);
    void pause();
    void stop();

protected:
    void timerEvent(QTimerEvent *);

private:
    Phonon::MediaObject *music;
    Phonon::AudioOutput *audioOutput;
    QLCDNumber *lcd;
    int totalTime;
    int bellTime;
    int endTime;
    int count;
    int timer;

    void display();

private slots:
    void musicState(Phonon::State, Phonon::State);
};

#endif
