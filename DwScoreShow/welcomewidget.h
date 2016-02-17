#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QLabel>

class WelcomeWidget : public QWidget
{
public:
    WelcomeWidget(QWidget* parent = 0);

    void setText(QString systemName, QString systemNameE,
                 QString organizers, QString organizersE,
                 QString contractors, QString contractorsE,
                 QString timePlace);
    void setFonts(QFont* font1, QFont* font2, QFont* font3);

private:
    QFont* fontH1;
    QFont* fontH2;
    QFont* fontH3;
    QLabel* labelSystemName;
    QLabel* labelSystemNameE;
    QLabel* labelOrganizers;
    QLabel* labelOrganizersE;
    QLabel* labelContractors;
    QLabel* labelContractorsE;
    QLabel* labelTimePlace;
};

#endif // WELCOMEWIDGET_H
