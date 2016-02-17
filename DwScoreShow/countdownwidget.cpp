#include <QVBoxLayout>
#include "countdownwidget.h"

CountDownWidget::CountDownWidget(QFont *font1, QFont *font2, QWidget *parent) :
    QWidget(parent)
{
    QString styleWhite(tr("color:rgb(255, 255, 255);"));
    QString styleRed(tr("color:rgb(255, 0, 0);"));

    this->labelInfo = new QLabel(this);
    this->labelInfo->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    this->labelInfo->setFont(*font1);
    this->labelInfo->setStyleSheet(styleWhite);
    this->labelInfo->setGeometry(0, 0, 1024, 130);

    this->countDown = new DwCountDown(this);
    this->countDown->setGeometry(200, 200, 624, 300);

    this->labelTip = new QLabel(this);
    this->labelTip->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->labelTip->setFont(*font2);
    this->labelTip->setStyleSheet(styleWhite);
    this->labelTip->setWordWrap(true);
    this->labelTip->setGeometry(130, 570, 550, 100);

    this->labelState = new QLabel(this);
    this->labelState->setFont(*font1);
    this->labelState->setStyleSheet(styleRed);
    this->labelState->setGeometry(830, 430, 150, 50);
}
