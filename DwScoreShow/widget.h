#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include "dwcountdown.h"
#include "welcomewidget.h"
#include "countdownwidget.h"
#include "scorewidget.h"
#include "rankwidget.h"

#define DW_PORT 8888

class QDomElement;
class QStackedWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    void showFullScreenWithResolution();
    void setSocket(QTcpSocket*);

protected:
    void closeEvent(QCloseEvent *);

private:
    QTcpSocket* socket;
    QFont* fontH1;
    QFont* fontH2;
    QFont* fontH3;
    QStackedWidget* widgetMain;
    WelcomeWidget* widgetWelcome;
    CountDownWidget* widgetCountDown;
    ScoreWidget* widgetScore;
    RankWidget* widgetRank;

    void getSystemInfo();
    void setupUi();

private slots:
    void error(QAbstractSocket::SocketError);
    void readInfo();
};

#endif // WIDGET_H
