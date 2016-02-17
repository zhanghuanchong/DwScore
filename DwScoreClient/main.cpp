#include <QtGui/QApplication>
#include <QTextCodec>
#include "clientdialog.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB18030"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB18030"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));

    QTcpSocket* socket = new QTcpSocket;
    LoginDialog* login = new LoginDialog;
    login->setSocket(socket);
    if(login->exec() == QDialog::Accepted)
    {
        ClientDialog* mainWidget = new ClientDialog;
        mainWidget->idClient = login->idClient;
        mainWidget->setSocket(socket);
        mainWidget->show();
        delete login;
    }
    else
    {
        return 0;
    }

    return a.exec();
}
