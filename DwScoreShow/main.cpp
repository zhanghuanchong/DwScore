#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QTextCodec>
#include <QHBoxLayout>
#include "widget.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QApplication a(argc, argv);
    QTcpSocket* socket = new QTcpSocket;
    LoginDialog* login = new LoginDialog;
    login->setSocket(socket);
    if(login->exec() == QDialog::Accepted)
    {
        delete login;
        Widget* mainWidget = new Widget;
        mainWidget->setSocket(socket);
        mainWidget->showFullScreenWithResolution();
    }
    else
    {
        return 0;
    }

    return a.exec();
}
