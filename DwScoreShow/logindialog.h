#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QDialog>
#include <QTcpSocket>

class QLineEdit;
class QPushButton;

class LoginDialog : public QDialog
{
    Q_OBJECT

private:
    QLineEdit* editIP;
    QLineEdit* editPwd;
    QLineEdit* editPort;
    QPushButton* btnConnect;
    QTcpSocket* socket;

private:
    void setupUi();

private slots:
    void connectServer();
    void login();
    void readInfo();
    void error(QAbstractSocket::SocketError);

public:
    explicit LoginDialog(QDialog *parent = 0);
    void setSocket(QTcpSocket*);

};

#endif // LOGINWIDGET_H
