#include "dwcountdown.h"
#include <QApplication>
#include <QPainter>
#include <QHBoxLayout>
#include <QMessageBox>

DwCountDown::DwCountDown(QWidget *parent) :
    QWidget(parent), totalTime(0), bellTime(0), endTime(0), count(0)
{
    lcd = new QLCDNumber(this);
    lcd->setStyleSheet(tr("color:rgb(255, 255, 255);"));
    lcd->setFrameShape(QFrame::NoFrame);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(lcd);
    this->setLayout(layout);

    music = Phonon::createPlayer(Phonon::MusicCategory);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(music, audioOutput);
    audioOutput->setVolume(100);
    connect(music,SIGNAL(stateChanged(Phonon::State, Phonon::State)),this,SLOT(musicState(Phonon::State, Phonon::State)));
}

void DwCountDown::display()
{
    int value;
    if(count <= totalTime)
    {
        value = totalTime - count;
        lcd->setStyleSheet(tr("color:rgb(255, 255, 255);"));
    }
    else
    {
        value = count - totalTime;
        lcd->setStyleSheet(tr("color:rgb(255, 0, 0);"));
    }
    int i = 4;
    if(value >= 600)
    {
        i = 5;
    }

    lcd->setDigitCount(i);

    int a = value / 60;
    int b = value % 60;
    QString str;
    if(b<10)
    {
        str = "%1:0%2";
    }else{
        str = "%1:%2";
    }

    str = str.arg(a).arg(b);

    lcd->display(str);
}

void DwCountDown::timerEvent(QTimerEvent *)
{
    if(count < endTime)
    {
        count++;
        if(this->bellTime == count){
            music->setCurrentSource(Phonon::MediaSource(QApplication::applicationDirPath() + "/alert.wav"));
            music->play();
        }
    }
    else
    {
        stop();
        music->setCurrentSource(Phonon::MediaSource(QApplication::applicationDirPath() + "/ringin.wav"));
        music->play();
    }
    display();
    this->repaint();
}

void DwCountDown::start()
{
    if(totalTime == 0 || count == endTime)
        return;
    pause();
    timer = startTimer(1000);
}

void DwCountDown::start(int t,int b, int e)
{
    pause();
    this->totalTime = t;
    this->bellTime = b;
    this->endTime = e;
    count = 0;
    display();
    timer = startTimer(1000);
}

void DwCountDown::pause()
{
    killTimer(timer);
}

void DwCountDown::stop()
{
    count = endTime;
    pause();
}

void DwCountDown::musicState(Phonon::State s, Phonon::State)
{
    if(Phonon::ErrorState == s)
    {
        QMessageBox::critical(this,"error",music->errorString());
    }
}
