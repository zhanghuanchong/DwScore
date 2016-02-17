#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QtGui/QDialog>
#include <QtNetwork/QAbstractSocket>
#include <QDomElement>
#include <QTableWidget>
#include <QTcpSocket>

class ClientDialog : public QDialog
{
    Q_OBJECT

private:
    QTableWidget* table;
    QPushButton* btnSubmit;
    QPushButton* btnModify;
    QTcpSocket* socket;

public:
    ClientDialog(QWidget *parent = 0);
    void setSocket(QTcpSocket*);
    int idClient;

private slots:
    void readInfo();
    void error(QAbstractSocket::SocketError);
    void showModifyButton(int,int,int,int);
    void submit();
    void modify();
    void submitModify();

private:
    void setupUi();
    void insertRow(int order, QString name = QString(), QString value = QString());
    void mark(int, QString);
    void setRowEditable(int, bool);
    void setItemEditable(int, int, bool);
    void initData(QDomElement);

protected:
    void keyPressEvent ( QKeyEvent * e );
};

#endif // CLIENTDIALOG_H
