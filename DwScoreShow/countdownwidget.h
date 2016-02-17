#ifndef COUNTDOWNWIDGET_H
#define COUNTDOWNWIDGET_H

#include <QWidget>
#include <QLabel>
#include "dwcountdown.h"

class CountDownWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CountDownWidget(QFont*, QFont*, QWidget *parent = 0);

    DwCountDown* countDown;
    QLabel* labelInfo;
    QLabel* labelTip;
    QLabel* labelState;
};

#endif // COUNTDOWNWIDGET_H
