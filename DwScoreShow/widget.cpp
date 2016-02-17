#include <QMessageBox>
#include <QtNetwork/QHostAddress>
#include <QPainter>
#include <QApplication>
#include <QCloseEvent>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QtXml>
#include "widget.h"
#include "globals.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    fontH1 = new QFont(tr("文泉驿微米黑"), 45, QFont::Bold);
    fontH2 = new QFont(tr("文泉驿微米黑"), 20, QFont::Bold);
    fontH3 = new QFont(tr("文泉驿微米黑"), 14, QFont::Bold);

    setupUi();
}

void Widget::setupUi()
{
    QPalette p = palette();
    QPixmap img(QApplication::applicationDirPath() + "/bg.jpg");
    p.setBrush(backgroundRole(), QBrush(img));
    setPalette(p);

    widgetMain = new QStackedWidget(this);
    widgetMain->setGeometry(0, 0, 1024, 768);

    widgetWelcome = new WelcomeWidget;
    widgetWelcome->setFonts(fontH1, fontH2, fontH3);
    widgetMain->addWidget(widgetWelcome);

    widgetCountDown = new CountDownWidget(fontH2, fontH3);
    widgetMain->addWidget(widgetCountDown);

    widgetScore = new ScoreWidget(fontH2);
    widgetMain->addWidget(widgetScore);

    widgetRank = new RankWidget(fontH2);
    widgetMain->addWidget(widgetRank);
}

void Widget::readInfo()
{
    QString str(tr(socket->readAll()));
    QDomDocument dom;
    dom.setContent(str);
    qDebug()<<str<<endl;
    QDomElement node = dom.documentElement();
    QString name =  node.nodeName();
    if(name == "systemInfo")
    {
        widgetWelcome->setText(node.firstChildElement("systemName").text(),
                                     node.firstChildElement("systemNameE").text(),
                                     tr("主办单位：").append(node.firstChildElement("organizers").text()),
                                     node.firstChildElement("organizersE").text(),
                                     tr("协办单位：").append(node.firstChildElement("contractors").text()),
                                     node.firstChildElement("contractorsE").text(),
                                     node.firstChildElement("time").text().append(tr("　"))
                                        .append(node.firstChildElement("timeE").text()).append(tr("　　"))
                                        .append(node.firstChildElement("place").text()).append(tr("　"))
                                        .append(node.firstChildElement("placeE").text()));
    }
    else if(name == "showWelcome")
    {
        widgetMain->setCurrentWidget(widgetWelcome);
    }
    else if(name == "showCountDown")
    {
        widgetCountDown->labelInfo->setText(node.firstChildElement("info").text());
        int total = node.firstChildElement("totalTime").text().toInt();
        int bell = node.firstChildElement("bellTime").text().toInt();
        int end = node.firstChildElement("endTime").text().toInt();
        widgetCountDown->labelTip->setText(tr("本环节时间为%1秒，用时不得短于%2秒（提示音“叮”），不得长于%3秒（提示音“叮呤呤”），否则予以适当扣分。")
                                                 .arg(total).arg(bell).arg(end));
        widgetCountDown->labelState->clear();
        widgetMain->setCurrentWidget(widgetCountDown);
        widgetCountDown->countDown->start(total, bell, end);
    }
    else if(name == "pauseCountDown")
    {
        if(widgetMain->currentWidget() == widgetCountDown)
        {
            widgetCountDown->countDown->pause();
            widgetCountDown->labelState->setText(tr("暂停计时"));
        }
    }
    else if(name == "continueCountDown")
    {
        if(widgetMain->currentWidget() == widgetCountDown)
        {
            widgetCountDown->countDown->start();
            widgetCountDown->labelState->clear();
        }
    }
    else if(name == "stopCountDown")
    {
        if(widgetMain->currentWidget() == widgetCountDown)
        {
            widgetCountDown->countDown->stop();
            widgetCountDown->labelState->setText(tr("停止计时"));
        }
    }
    else if(name == "showScore")
    {
        widgetScore->labelInfo->setText(node.firstChildElement("info").text());
        int juryCount = node.firstChildElement("juryCount").text().toInt();
        QStringList strSide = node.firstChildElement("huanjie").text().split('|');
        widgetScore->setTable(juryCount, strSide, fontH2, fontH3);
        widgetMain->setCurrentWidget(widgetScore);
        widgetScore->labelTip->setText(tr("请评委打分！"));
        widgetScore->labelTip->setFocus();
        socket->write(tr("<getScoreData />").toAscii());
    }
    else if(name == "scoreData")
    {
        QDomNodeList scores = node.elementsByTagName("score");
        QTableWidget* table = widgetScore->table;
        QAbstractItemModel* model = table->model();
        for(int i=0; i<scores.size(); i++)
        {
            QDomNode tempNode = scores.at(i);
            if(tempNode.nodeType() == QDomNode::ElementNode)
            {
                QDomElement element = tempNode.toElement();
                QStringList list = element.text().split("|");
                for(int j=0; j<list.size(); j++)
                {
                    model->setData(model->index(j, i), list.at(j));
                    table->item(j, i)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
        if(scores.size() == table->columnCount()-1)
        {
            double average = 0.0, averageShow = 0.0;
            for(int i=0; i < table->rowCount(); i++)
            {
                double min = 1e10, max = 0.0, averageRow = 0.0;
                int indexMin = 0, indexMax = 0;
                for(int j=0; j < table->columnCount()-1; j++)
                {
                    double value = table->item(i, j)->text().toDouble();
                    if(value < min)
                    {
                        min = value;
                        indexMin = j;
                    }
                    if(value > max)
                    {
                        max = value;
                        indexMax = j;
                    }
                    averageRow += value;
                }
                averageRow -= table->item(i, indexMin)->text().toDouble();
                averageRow -= table->item(i, indexMax)->text().toDouble();
                averageRow /= table->columnCount()-3;
                average += averageRow;
                averageRow = tr("%1").arg(averageRow, 0, 'f', 2).toDouble();
                averageShow += averageRow;
                model->setData(model->index(i, table->columnCount()-1), averageRow);
                table->item(i, table->columnCount() -1)->setTextAlignment(Qt::AlignCenter);
                table->item(i, table->columnCount() -1)->setBackground(QBrush(table->palette().button()));
                table->item(i, indexMin)->setTextColor(Qt::red);
                table->item(i, indexMax)->setTextColor(Qt::blue);
            }
            widgetScore->labelTip->setTextFormat(Qt::RichText);
            widgetScore->labelTip->setText(tr("去掉每个环节的最高分（蓝色）和最低分（红色），该选手本轮得分：<br/><strong style='font-size:120px;border-bottom:10px solid white'>%1</strong>").arg(averageShow));
            socket->write(tr("<lunciScore>%1</lunciScore>").arg(average, 0, 'f', 4).toAscii());
        }
    }
    else if(name == "showRank")
    {
        widgetRank->start(&node);
        widgetMain->setCurrentWidget(widgetRank);
    }
}

void Widget::getSystemInfo()
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        QString str("<getSystemInfo />");
        socket->write(str.toAscii());
    }
}

void Widget::showFullScreenWithResolution()
{
    //showFullScreen();
    //setResolution(1024, 768);
    setGeometry(0, 25, 1024, 768);
    show();
    getSystemInfo();
}

void Widget::closeEvent(QCloseEvent *)
{
    //setResolution(1280, 1024);
}

void Widget::setSocket(QTcpSocket *pSocket)
{
    socket = pSocket;
    connect(socket, SIGNAL(readyRead()), this, SLOT(readInfo()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error(QAbstractSocket::SocketError)));
}

void Widget::error(QAbstractSocket::SocketError err)
{
    QString strErr = tr("连接服务器失败！错误代码：%1。").arg(err);
    if(QAbstractSocket::HostNotFoundError == err)
    {
        QMessageBox::information(this,tr("错误"),strErr.append(tr("在当前网络中未找到服务器，请确保服务器已经启动并处于监听状态。")));
    }
    else if(QAbstractSocket::ConnectionRefusedError == err)
    {
        QMessageBox::information(this,tr("错误"),strErr.append(tr("连接被拒绝，服务器未响应，访问超时！")));
    }
    else if(QAbstractSocket::RemoteHostClosedError == err)
    {
        QMessageBox::information(this,tr("错误"),strErr.append(tr("服务器已关闭！")));
    }
    else
    {
        QMessageBox::information(this,tr("错误"),strErr);
    }
}
