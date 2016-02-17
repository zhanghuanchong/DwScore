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
    editName = new QLineEdit(tr("pw1"));
    editPwd = new QLineEdit(tr("123456"));
    editPwd->setEchoMode(QLineEdit::Password);
    btnConnect = new QPushButton(tr("登录"));
    connect(btnConnect, SIGNAL(clicked()), this, SLOT(connectServer()));

    QFormLayout* layoutForm = new QFormLayout;
    layoutForm->addRow(tr("服务器IP："), editIP);
    layoutForm->addRow(tr("　　端口："), editPort);
    layoutForm->addRow(tr("　用户名："), editName);
    layoutForm->addRow(tr("　　密码："), editPwd);
    layoutForm->addWidget(btnConnect);

    this->setLayout(layoutForm);

    this->resize(260, 150);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    this->setWindowTitle(tr("DevWing打分系统评委端"));
    this->setModal(true);
}

void LoginDialog::connectServer()
{
    QString strIP = editIP->text().trimmed();
    if(strIP == "")
    {
        QMessageBox::critical(this, tr("错误"), tr("请输入服务器IP。"));
        return;
    }
    QString strPort = editPort->text().trimmed();
    if(strPort == "")
    {
        QMessageBox::critical(this, tr("错误"), tr("请输入端口号。"));
        return;
    }
    QString strName = editName->text().trimmed();
    if(strName == "")
    {
        QMessageBox::critical(this, tr("错误"), tr("请输入用户名。"));
        return;
    }
    QString strPwd = editPwd->text().trimmed();
    if(strPwd == "")
    {
        QMessageBox::critical(this, tr("错误"), tr("请输入登录密码。"));
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
    QString str("<login type='client' name='");
    str.append(editName->text().trimmed());
    str.append("'>");
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
            idClient = node.attribute("id").toInt();
            this->accept();
        }
        else if(result == "failed")
        {
            QMessageBox::critical(this, tr("错误"), tr("用户名或密码错误！"));
            this->setEnabled(true);
            this->editPwd->setFocus();
            this->editPwd->selectAll();
        }
        else
        {
            QMessageBox::critical(this, tr("错误"), tr("服务器出错！请联系开发人员。"));
            this->setEnabled(true);
        }
    }
}

void LoginDialog::error(QAbstractSocket::SocketError err)
{
    qDebug()<<"error number:"<<err<<endl;
    if(QAbstractSocket::ConnectionRefusedError == err)
    {
        QMessageBox::critical(this, tr("错误"), tr("未找到服务器。"), QMessageBox::Ok);
    }
    else if(QAbstractSocket::RemoteHostClosedError == err)
    {
        QMessageBox::critical(this, tr("错误"), tr("服务器已关闭。"), QMessageBox::Ok);
    }
    this->setEnabled(true);
}
