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
    QLineEdit* editPwd;
    QLineEdit* editPort;
    QPushButton* btnConnect;

private:
    void setupUi();

private slots:
    void login();

public:
    explicit LoginDialog(QDialog *parent = 0);
    quint16 getPort();

};

#endif // LOGINWIDGET_H
