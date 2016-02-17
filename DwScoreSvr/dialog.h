#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QList>
#include <QtSql>

#include "ui_form.h"

class QTextEdit;

class Dialog : public QDialog, public Ui_Form
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    void setDatabase(QSqlDatabase*);
    void listen(quint16 port);

    enum State{None, Welcome, CountDown, Score};

private:
    QTcpServer* tcp;
    QSqlDatabase* db;
    QList<QTcpSocket*> listSocketClient;
    QTcpSocket* socketShow;
    State state;

    void initFormData();
    void setBtnsEnabled(bool);
    void insertRow(QTcpSocket*, QString, int id = 0);
    void sendScoresToShow();

private slots:
    void on_btnOrder_clicked();
    void on_btnHuanjie_clicked();
    void on_btnChangci_clicked();
    void on_btnLunci_clicked();
    void on_btnWelcome_clicked();
    void on_btnCountDown_clicked();
    void on_btnCountPause_clicked();
    void on_btnCountStop_clicked();
    void on_btnScore_clicked();
    void on_btnInit_clicked();
    void on_btnRank_clicked();
    void acceptNewConn();
    void readMsg();
    void error(QAbstractSocket::SocketError);

protected:
    void keyPressEvent ( QKeyEvent * e );
};

#endif // DIALOG_H
