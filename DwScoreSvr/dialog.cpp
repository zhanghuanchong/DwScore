#include <QtNetwork/QTcpServer>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QHostInfo>
#include <QInputDialog>
#include <QFile>
#include <QKeyEvent>
#include "dialog.h"
#include "initwizard.h"

int getConfigInt(QString);
QString getConfigStr(QString);
bool setConfig(QString, QString);

Dialog::Dialog(QWidget *parent) :
    QDialog(parent), socketShow(0), state(None)
{
    setupUi(this);

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - width())/2, (desktop->height() - height())/2);

    setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    setWindowTitle(getConfigStr("systemName").append(windowTitle()));

    initFormData();

    setBtnsEnabled(false);

    QStringList list;
    list<<tr("计算机")<<tr("IP")<<tr("类型")<<tr("socket")<<tr("ID");
    tableClient->setColumnCount(list.size());
    tableClient->setHorizontalHeaderLabels(list);
    tableClient->resizeColumnsToContents();
    QFont font;
    font.setPixelSize(12);
    tableClient->setFont(font);
}

void Dialog::listen(quint16 port)
{
    tcp = new QTcpServer;
    if(tcp->listen(QHostAddress::Any, port))
    {
        strTip->setText(tr("监听中... 本机IP：")
                        .append(QHostInfo::fromName(QHostInfo::localHostName()).addresses().first().toString())
                        .append(tr("，监听端口：%1"))
                        .arg(port));
        connect(tcp,SIGNAL(newConnection()),this,SLOT(acceptNewConn()));
    }
    else
    {
        strTip->setText(tr("监听%1端口失败！请关掉不必要的应用程序，然后重新启动本服务器。").arg(port));
    }
}

void Dialog::setDatabase(QSqlDatabase *pDb)
{
    db = pDb;
}

void Dialog::initFormData()
{
    spinOrder->setValue(getConfigInt("currentOrder"));
    spinHuanjie->setValue(getConfigInt("currentHuanjie"));
    spinChangci->setValue(getConfigInt("currentChangci"));
    spinLunci->setValue(getConfigInt("currentLunci"));
}

void Dialog::insertRow(QTcpSocket *socket, QString str, int id)
{
    int row = tableClient->rowCount();
    tableClient->insertRow(row);
    QAbstractItemModel* model = tableClient->model();
    model->setData(model->index(row, 0), socket->peerName());
    model->setData(model->index(row, 1), socket->peerAddress().toString());
    model->setData(model->index(row, 2), str);
    model->setData(model->index(row, 3), tr("%1").arg(int(socket)));
    model->setData(model->index(row, 4), tr("%1").arg(id));
    tableClient->resizeColumnsToContents();
}

void Dialog::on_btnOrder_clicked()
{
    if(setConfig("currentOrder", spinOrder->text().trimmed()))
    {
        strTip->append(tr("成功修改当前选手序号！当前值：").append(getConfigStr("currentOrder")));
    }
}

void Dialog::on_btnHuanjie_clicked()
{
    if(setConfig("currentHuanjie", spinHuanjie->text().trimmed()))
    {
        strTip->append(tr("成功修改当前环节序号！当前值：").append(getConfigStr("currentHuanjie")));
    }
}

void Dialog::on_btnChangci_clicked()
{
    if(setConfig("currentChangci", spinChangci->text().trimmed()))
    {
        strTip->append(tr("成功修改当前场次序号！当前值：").append(getConfigStr("currentChangci")));
    }
}

void Dialog::on_btnLunci_clicked()
{
    if(setConfig("currentLunci", spinLunci->text().trimmed()))
    {
        strTip->append(tr("成功修改当前轮次序号！当前值：").append(getConfigStr("currentLunci")));
    }
}

void Dialog::on_btnWelcome_clicked()
{
    btnCountPause->setText(tr("暂停计时"));
    socketShow->write(tr("<showWelcome />").toAscii());
    socketShow->flush();
    state = Welcome;
}

void Dialog::on_btnCountDown_clicked()
{
    btnCountPause->setText(tr("暂停计时"));
    QString str("<showCountDown><info>");
    QSqlQuery query = db->exec(tr("select [name] from [lunci] where [order] = %1").arg(getConfigInt("currentLunci")));
    if(!query.next())
    {
        strTip->append(tr("查询当前轮次信息出错！"));
        return;
    }
    str.append(query.value(0).toString());

    str.append(tr("　第")).append(getConfigStr("currentChangci")).append(tr("场　"))
            .append(getConfigStr("currentOrder")).append(tr("号选手　"));

    query.exec(tr("select [name],[totalTime],[bellTime],[endTime] from [huanjie] where [order] = %1 and [lunci] = %2").arg(getConfigInt("currentHuanjie")).arg(getConfigInt("currentLunci")));
    if(!query.next())
    {
        strTip->append(tr("查询当前环节信息出错！"));
        return;
    }
    str.append(query.value(0).toString());

    str.append(tr("</info><totalTime>%1</totalTime><bellTime>%2</bellTime><endTime>%3</endTime></showCountDown>")
               .arg(query.value(1).toInt())
               .arg(query.value(2).toInt())
               .arg(query.value(3).toInt()));

    socketShow->write(str.toAscii());
    state = CountDown;
}

void Dialog::on_btnCountPause_clicked()
{
    if(btnCountPause->text() == tr("暂停计时"))
    {
        socketShow->write(tr("<pauseCountDown />").toAscii());
        btnCountPause->setText(tr("继续计时"));
    }
    else
    {
        socketShow->write(tr("<continueCountDown />").toAscii());
        btnCountPause->setText(tr("暂停计时"));
    }
    socketShow->flush();
}

void Dialog::on_btnCountStop_clicked()
{
    btnCountPause->setText(tr("暂停计时"));
    socketShow->write(tr("<stopCountDown />").toAscii());
    socketShow->flush();
}

void Dialog::on_btnScore_clicked()
{
    btnCountPause->setText(tr("暂停计时"));
    QString str("<showScore><info>");
    QSqlQuery query = db->exec(tr("select [name] from [lunci] where [order] = %1").arg(getConfigInt("currentLunci")));
    if(!query.next())
    {
        strTip->append(tr("查询当前轮次信息出错！"));
        return;
    }
    str.append(query.value(0).toString());

    str.append(tr("　第")).append(getConfigStr("currentChangci")).append(tr("场　"))
            .append(getConfigStr("currentOrder")).append(tr("号选手 Player ")).append(getConfigStr("currentOrder"));

    str.append(tr("</info><juryCount>%1</juryCount><huanjie>").arg(/*listSocketClient.size()*/5));
    query.exec(tr("select [name] from [huanjie] where [lunci] = %1 order by [order] asc").arg(getConfigInt("currentLunci")));
    while(query.next())
    {
        str.append(query.value(0).toString().append("|"));
    }
    str = str.left(str.length() - 1);
    str.append("</huanjie></showScore>");

    socketShow->write(str.toAscii());
    strTip->append(str);
    socketShow->flush();

    query.prepare("select [name] from [player] where [order] = :order and [lunci]=:lunci and [changci] = :changci");
    query.bindValue(":order", getConfigInt("currentOrder"));
    query.bindValue(":lunci", getConfigInt("currentLunci"));
    query.bindValue(":changci", getConfigInt("currentChangci"));
    query.exec();
    QString name;
    if(query.next())
    {
        name = query.value(0).toString();
    }
    for(int i=0; i<listSocketClient.size(); i++)
    {
        listSocketClient.at(i)->write(tr("<mark order='%1' name='%2' />")
                                      .arg(getConfigInt("currentOrder"))
                                      .arg(name).toAscii());
    }
    state = Score;
}

void Dialog::on_btnInit_clicked()
{
    QInputDialog input;
    input.setLabelText(tr("请输入管理员密码："));
    input.setTextValue("123456");
    input.setTextEchoMode(QLineEdit::Password);
    input.setWindowTitle(this->windowTitle());
    if(input.exec())
    {
        if(input.textValue() == getConfigStr("adminPwd"))
        {
            InitWizard initWizard(db);
            if(initWizard.exec())
            {
                setWindowTitle(getConfigStr("systemName").append(windowTitle()));
                initFormData();
            }
        }
        else
        {
            QMessageBox::critical(this, tr("错误"), tr("密码不正确！"));
        }
    }
}

void Dialog::on_btnRank_clicked()
{
    QString str(tr("<showRank lunciName='"));
    QSqlQuery query(*db);
    query.exec(tr("select [name] from [lunci] where [order] = ").append(getConfigStr("currentLunci")));
    if(query.next())
    {
        str.append(query.value(0).toString()).append("'>");
    }
    else
    {
        strTip->append(tr("查询轮次信息失败！"));
        return;
    }
    query.prepare("select [name], [order], [changci], [score] from [player] where [lunci]=:lunci order by [score] desc");
    query.bindValue(":lunci", getConfigInt("currentLunci"));
    query.exec();
    int i = 1;
    double last = 0.0;
    while(query.next())
    {
        i++;
        double score = query.value(3).toDouble();
        if(last == score)
        {
            i--;
        }
        last = score;
        str.append(tr("<player id='%1' name='%2' order='%3' changci='%4' score='%5'/>")
                   .arg(i)
                   .arg(query.value(0).toString())
                   .arg(query.value(1).toInt())
                   .arg(query.value(2).toInt())
                   .arg(score));
    }
    str.append("</showRank>");
    socketShow->write(str.toAscii());
    strTip->append(str);
}

void Dialog::acceptNewConn()
{
    strTip->append(tr("收到新客户端的连接..."));
    QTcpSocket* socket = tcp->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMsg()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
}

void Dialog::readMsg()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString str(tr(socket->readAll()));
    QDomDocument dom;
    dom.setContent(str);
    strTip->append(tr("收到客户端信息：").append(str));
    QDomElement node = dom.documentElement();
    QString name =  node.nodeName();
    if(name == "getInitInfo")
    {
        QString strInit;
        QString sql = QString("select [min],[max],[order],[name] from [huanjie] where [lunci] = %1 order by [order] asc").arg(getConfigInt(tr("currentLunci")));
        QSqlQuery query = db->exec(sql);
        strInit.append("<initInfo>");
        while(query.next())
        {
            strInit.append(tr("<huanjie min='%1' max='%2' order='%3'>%4</huanjie>")
                        .arg(query.value(0).toString())
                        .arg(query.value(1).toString())
                        .arg(query.value(2).toString())
                        .arg(query.value(3).toString()));
        }
        strInit.append("<systemName>").append(getConfigStr("systemName")).append("</systemName>");
        strInit.append("<currentLunci>").append(getConfigStr("currentLunci")).append("</currentLunci>");
        strInit.append("<currentChangci>").append(getConfigStr("currentChangci")).append("</currentChangci>");
        strInit.append("<scoreData>");
        query.prepare("select [playerOrder], [score].[score], [name] from [score],[player] where [juryID]=:juryID and [score].[changci]=:changci and [player].[changci]=[score].[changci] and [playerOrder]=[order] and [score].[lunci]=:lunci and [score].[lunci]=[player].[lunci] order by [playerOrder] asc, [huanjie] asc");
        query.bindValue(":juryID", node.attribute("id").toInt());
        query.bindValue(":changci", getConfigInt("currentChangci"));
        query.bindValue(":lunci", getConfigInt("currentLunci"));
        query.exec();
        int i = 0;
        while(query.next())
        {
            int order = query.value(0).toInt();
            double value = query.value(1).toDouble();
            QString player = query.value(2).toString();
            if(order != i)
            {
                if(i != 0)
                {
                    strInit.append("</score>");
                }
                strInit.append(tr("<score order='%1' name='%2'>%3").arg(order).arg(player).arg(value));
                i = order;
            }
            else
            {
                strInit.append(tr("|%1").arg(value));
            }
        }
        if(i != 0)
        {
            strInit.append("</score>");
        }
        strInit.append("</scoreData>");
        if(state == Score)
        {
            query.prepare("select [name] from [player] where [order] = :order and [lunci]=:lunci and [changci] = :changci");
            query.bindValue(":order", getConfigInt("currentOrder"));
            query.bindValue(":lunci", getConfigInt("currentLunci"));
            query.bindValue(":changci", getConfigInt("currentChangci"));
            query.exec();
            QString player;
            if(query.next())
            {
                player = query.value(0).toString();
            }
            strInit.append(tr("<mark order='%1' name='%2' />")
                                          .arg(getConfigInt("currentOrder"))
                                          .arg(player).toAscii());
        }
        strInit.append("</initInfo>");
        strTip->append(tr("发送到%1评委端的初始数据：%2").arg((int)socket).arg(strInit));
        socket->write(strInit.toAscii());
    }
    else if(name == "login")
    {
        QString type = node.attribute("type");
        if(type == "show")
        {
            QString pwd = getConfigStr("showPwd");
            if(pwd != "")
            {
                if(node.text() == pwd)
                {
                    if(socketShow != 0 && socketShow->state() == QAbstractSocket::ConnectedState)
                    {
                        socket->write(tr("<login>refused</login>").toAscii());
                    }
                    else
                    {
                        socket->write(tr("<login>success</login>").toAscii());
                        socketShow = socket;
                        strTip->append(tr("展示端已成功连接..."));
                        setBtnsEnabled(true);
                        insertRow(socket, tr("展示端"));
                        state = Welcome;
                    }
                }
                else
                {
                    socket->write(tr("<login>failed</login>").toAscii());
                }
            }
            else
            {
                strTip->append(tr("检索展示端密码失败！"));
                socket->write(tr("<login>error</login>").toAscii());
            }
        }
        else
        {
            QSqlQuery query(*db);
            query.prepare("select [pwd],[id] from [jury] where [name]=:name");
            query.bindValue(":name", node.attribute("name"));
            query.exec();
            if(query.next() && query.value(0).toString() == node.text())
            {
                socket->write(tr("<login id='%1'>success</login>").arg(query.value(1).toInt()).toAscii());
                listSocketClient.append(socket);
                strTip->append(tr("新评委端已连接..."));
                insertRow(socket, tr("评委端").append(node.attribute("name")), query.value(1).toInt());
            }
            else
            {
                socket->write(tr("<login>failed</login>").toAscii());
            }
        }
    }
    else if(name == "getSystemInfo")
    {
        QString str("<systemInfo>");

        str.append("<systemName>").append(getConfigStr("systemName")).append("</systemName>");
        str.append("<systemNameE>").append(getConfigStr("systemNameE")).append("</systemNameE>");
        str.append("<organizers>").append(getConfigStr("organizers")).append("</organizers>");
        str.append("<organizersE>").append(getConfigStr("organizersE")).append("</organizersE>");
        str.append("<contractors>").append(getConfigStr("contractors")).append("</contractors>");
        str.append("<contractorsE>").append(getConfigStr("contractorsE")).append("</contractorsE>");
        str.append("<time>").append(getConfigStr("time")).append("</time>");
        str.append("<timeE>").append(getConfigStr("timeE")).append("</timeE>");
        str.append("<place>").append(getConfigStr("place")).append("</place>");
        str.append("<placeE>").append(getConfigStr("placeE")).append("</placeE>");

        str.append("</systemInfo>");
        socket->write(str.toAscii());
    }
    else if(name == "submit")
    {
        int idClient = node.attribute("id").toInt();
        int player = node.attribute("player").toInt();
        int changci = getConfigInt("currentChangci");
        int lunci = getConfigInt("currentLunci");
        QDomNodeList list = node.childNodes();
        for(int i=0; i<list.size(); i++)
        {
            if(list.at(i).nodeType() == QDomNode::ElementNode)
            {
                QDomElement element = list.at(i).toElement();
                db->exec(tr("insert into [score]([playerOrder],[changci],[lunci],[huanjie],[juryID],[score]) values(%1,%2,%3,%4,%5,%6)")
                         .arg(player).arg(changci).arg(lunci).arg(element.attribute("order")).arg(idClient).arg(element.text()));
            }
        }
        if(state == Score)
        {
            sendScoresToShow();
        }
    }
    else if(name == "submitModify")
    {
        int idClient = node.attribute("id").toInt();
        int player = node.attribute("player").toInt();
        int changci = getConfigInt("currentChangci");
        int lunci = getConfigInt("currentChangci");
        QDomNodeList list = node.childNodes();
        for(int i=0; i<list.size(); i++)
        {
            if(list.at(i).nodeType() == QDomNode::ElementNode)
            {
                QDomElement element = list.at(i).toElement();
                db->exec(tr("update [score] set [score] = %6 where [playerOrder] = %1 and [changci] = %2 and [lunci] = %3 and [huanjie] = %4 and [juryID] = %5")
                         .arg(player).arg(changci).arg(lunci).arg(element.attribute("order")).arg(idClient).arg(element.text()));
            }
        }
        if(state == Score)
        {
            sendScoresToShow();
        }
    }
    else if(name == "getScoreData")
    {
        sendScoresToShow();
    }
    else if(name == "lunciScore")
    {
        double value = node.text().toDouble();
        QSqlQuery query(*db);
        query.prepare("update [player] set [score]=:score where [order]=:order and [lunci]=:lunci and [changci]=:changci");
        query.bindValue(":score", value);
        query.bindValue(":order", getConfigInt("currentOrder"));
        query.bindValue("lunci", getConfigInt("currentLunci"));
        query.bindValue(":changci", getConfigInt("currentChangci"));
        query.exec();
    }
}

void Dialog::sendScoresToShow()
{
    QSqlQuery query(*db);
    query.prepare(tr("select [juryID],[score] from [score] where [playerOrder]=:order and [changci]=:changci and [lunci]=:lunci order by [juryID] asc, [huanjie] asc"));
    query.bindValue(":order", getConfigInt("currentOrder"));
    query.bindValue(":changci", getConfigInt("currentChangci"));
    query.bindValue(":lunci", getConfigInt("currentLunci"));
    query.exec();
    QString str("<scoreData>");
    int i=0, count=0;
    while(query.next())
    {
        int juryID = query.value(0).toInt();
        QString score = query.value(1).toString();
        if(juryID != i)
        {
            if(i != 0)
            {
                str.append("</score>");
            }
            str.append(tr("<score juryID='%1'>%2").arg(juryID).arg(score));
            i = juryID;
            count++;
        }
        else
        {
            str.append(tr("|%1").arg(score));
        }
    }
    if(i != 0)
    {
        str.append("</score>");
    }
    str.append("</scoreData>");
    strTip->append(str);
    socketShow->write(str.toAscii());
}

void Dialog::setBtnsEnabled(bool on)
{
    btnWelcome->setEnabled(on);
    btnCountDown->setEnabled(on);
    btnCountPause->setEnabled(on);
    btnCountStop->setEnabled(on);
    btnScore->setEnabled(on);
    btnRank->setEnabled(on);
}

void Dialog::error(QAbstractSocket::SocketError err)
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString strErr = tr("出现错误，代码：%1。").arg(err);
    if(QTcpSocket::RemoteHostClosedError == err){
        strTip->append(tr("客户端%1断开连接！").arg(socket->peerAddress().toString()));
        if(socket == socketShow)
        {
            setBtnsEnabled(false);
            strTip->append(tr("展示端断开连接！"));
            state = None;
        }
        else
        {
            for(int i=0; i<listSocketClient.size(); i++)
            {
                QTcpSocket* s = listSocketClient.at(i);
                if(s == socket)
                {
                    listSocketClient.removeAt(i);
                    break;
                }
            }
        }
        QString ip = socket->peerAddress().toString();
        int addSocket = int(socket);
        for(int i=0; i<tableClient->rowCount(); i++)
        {
            QTableWidgetItem* item = tableClient->item(i, 3);
            if(item->text().toInt() == addSocket)
            {
                tableClient->model()->removeRow(item->row());
            }
        }
    }
    else
    {
        strTip->append(strErr);
    }
}

void Dialog::keyPressEvent(QKeyEvent *)
{
}
