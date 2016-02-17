#include <QStyle>
#include "welcomewidget.h"

WelcomeWidget::WelcomeWidget(QWidget* parent)
    : QWidget(parent)
{
    QString strCSS(tr("color:rgb(255,255,255);"));
    this->labelSystemName = new QLabel(this);
    this->labelSystemName->setGeometry(0, 0, 1024, 250);
    this->labelSystemName->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    this->labelSystemName->setStyleSheet(strCSS);

    this->labelSystemNameE = new QLabel(this);
    this->labelSystemNameE->setGeometry(0, 260, 1024, 50);
    this->labelSystemNameE->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    this->labelSystemNameE->setStyleSheet(strCSS);

    this->labelOrganizers = new QLabel(this);
    this->labelOrganizers->setGeometry(200, 430, 824, 50);
    this->labelOrganizers->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->labelOrganizers->setStyleSheet(strCSS);

    this->labelOrganizersE = new QLabel(this);
    this->labelOrganizersE->setGeometry(340, 460, 824, 60);
    this->labelOrganizersE->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->labelOrganizersE->setStyleSheet(strCSS);

    this->labelContractors = new QLabel(this);
    this->labelContractors->setGeometry(200, 520, 824, 50);
    this->labelContractors->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->labelContractors->setStyleSheet(strCSS);

    this->labelContractorsE = new QLabel(this);
    this->labelContractorsE->setGeometry(340, 550, 824, 60);
    this->labelContractorsE->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->labelContractorsE->setStyleSheet(strCSS);

    this->labelTimePlace = new QLabel(this);
    this->labelTimePlace->setGeometry(0, 610, 1024, 50);
    this->labelTimePlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->labelTimePlace->setStyleSheet(strCSS);
}

void WelcomeWidget::setFonts(QFont *font1, QFont *font2, QFont *font3)
{
    this->fontH1 = font1;
    this->fontH2 = font2;
    this->fontH3 = font3;
    this->labelSystemName->setFont(*(this->fontH1));
    this->labelSystemNameE->setFont(*(this->fontH2));
    this->labelOrganizers->setFont(*(this->fontH2));
    this->labelOrganizersE->setFont(*(this->fontH3));
    this->labelContractors->setFont(*(this->fontH2));
    this->labelContractorsE->setFont(*(this->fontH3));
    this->labelTimePlace->setFont(*(this->fontH3));
}

void WelcomeWidget::setText(QString systemName, QString systemNameE,
                            QString organizers, QString organizersE,
                            QString contractors, QString contractorsE,
                            QString timePlace)
{
    this->labelSystemName->setText(systemName);
    this->labelSystemNameE->setText(systemNameE);
    this->labelOrganizers->setText(organizers);
    this->labelOrganizersE->setText(organizersE);
    this->labelContractors->setText(contractors);
    this->labelContractorsE->setText(contractorsE);
    this->labelTimePlace->setText(timePlace);
}
