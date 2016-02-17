#include <QApplication>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QHostAddress>
#include <QHostInfo>
#include <QtXml/QDomElement>
#include "logindialog.h"

LoginDialog::LoginDialog(QDialog *parent) :
    QDialog(parent, Qt::Widget | Qt::WindowMinimizeButtonHint)
{
    setupUi();
}

void LoginDialog::setSocket(QTcpSocket *pSocket)
{
    this->socket = pSocket;
    connect(socket, SIGNAL(connected()), this, SLOT(login()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readInfo()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

void LoginDialog::setupUi()
{
    //editIP = new QLineEdit(tr("210.42.242.88"));
    editIP = new QLineEdit(QHostInfo::fromName(QHostInfo::localHostName()).addresses().first().toString());
    editPort = new QLineEdit(tr("8888"));
    editPwd = new QLineEdit(tr("123456"));
    editPwd->setEchoMode(QLineEdit::Password);
    btnConnect = new QPushButton(tr("µÇÂ¼"));
    connect(btnConnect, SIGNAL(clicked()), this, SLOT(connectServer()));

    QFormLayout* layoutForm = new QFormLayout;
    layoutForm->addRow(tr("·þÎñÆ÷IP£º"), editIP);
    layoutForm->addRow(tr("¡¡¡¡¶Ë¿Ú£º"), editPort);
    layoutForm->addRow(tr("¡¡¡¡ÃÜÂë£º"), editPwd);
    layoutForm->addWidget(btnConnect);

    this->setLayout(layoutForm);

    this->resize(260, 120);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    this->setWindowTitle(tr("DevWing´ò·ÖÏµÍ³Õ¹Ê¾¶Ë"));
    this->setModal(true);
}

void LoginDialog::connectServer()
{
    QString strIP = editIP->text().trimmed();
    if(strIP == "")
    {
        QMessageBox::critical(this, tr("´íÎó"), tr("ÇëÊäÈë·þÎñÆ÷IP¡£"));
        return;
    }
    QString strPort = editPort->text().trimmed();
    if(strPort == "")
    {
        QMessageBox::critical(this, tr("´íÎó"), tr("ÇëÊäÈë¶Ë¿ÚºÅ¡£"));
        return;
    }
    QString strPwd = editPwd->text().trimmed();
    if(strPwd == "")
    {
        QMessageBox::critical(this, tr("´íÎó"), tr("ÇëÊäÈëµÇÂ¼ÃÜÂë¡£"));
        return;
    }
    this->setEnabled(false);
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        login();
    }
    else
    {
        socket->connectToHost(QHostAddress(strIP), strPort.toInt());
    }
}

void LoginDialog::login()
{
    QString str("<login type='show'>");
    str.append(editPwd->text().trimmed());
    str.append("</login>");
    socket->write(str.toAscii());
}

void LoginDialog::readInfo()
{
    QString str(socket->readAll());
    QDomDocument dom;
    dom.setContent(str);
    QDomElement node = dom.documentElement();
    QString name =  node.nodeName();
    if(name == "login")
    {
        QString result = node.text();
        if(result == "success")
        {
            this->accept();
        }
        else if(result == "failed")
        {
            QMessageBox::critical(this, tr("´íÎó"), tr("ÃÜÂë´íÎó£¡"));
            this->setEnabled(true);
            this->editPwd->setFocus();
            this->editPwd->selectAll();
        }
        else if(result == "refused")
        {
            QMessageBox::critical(this, tr("´íÎó"), tr("ÒÑ¾­ÓÐÒ»¸öÕ¹Ê¾¶ËµÇÂ¼ÁË¡£"));
            this->setEnabled(true);
        }
    }
}

void LoginDialog::error(QAbstractSocket::SocketError err)
{
    qDebug()<<"error number:"<<err<<endl;
    if(QAbstractSocket::ConnectionRefusedError == err)
    {
        QMessageBox::critical(this, tr("´íÎó"), tr("Î´ÕÒµ½·þÎñÆ÷¡£"), QMessageBox::Ok);
    }
    else if(QAbstractSocket::RemoteHostClosedError == err)
    {
        QMessageBox::critical(this, tr("´íÎó"), tr("·þÎñÆ÷ÒÑ¹Ø±Õ¡£"), QMessageBox::Ok);
    }
    this->setEnabled(true);
}
