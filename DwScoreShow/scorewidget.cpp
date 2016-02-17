#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include "scorewidget.h"

ScoreWidget::ScoreWidget(QFont* font, QWidget *parent) :
    QWidget(parent)
{
    QString styleWhite(tr("color:rgb(255, 255, 255);"));

    labelInfo = new QLabel(this);
    labelInfo->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    labelInfo->setFont(*font);
    labelInfo->setStyleSheet(styleWhite);
    labelInfo->setGeometry(0, 0, 1024, 130);

    table = new QTableWidget(this);
    table->setGeometry(50, 200, 924, 200);

    labelTip = new QLabel(this);
    labelTip->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    labelTip->setFont(*font);
    labelTip->setStyleSheet(styleWhite);
    labelTip->setGeometry(0, 450, 1024, 180);
}

void ScoreWidget::setTable(int columnCount, QStringList strSide, QFont* font, QFont* fontSmall)
{
    // set horizontal header
    table->setColumnCount(columnCount + 1);
    QStringList strHeader;
    for(int i=0; i<columnCount; i++)
    {
        strHeader<<tr("ÆÀÎ¯%1").arg(i+1);
    }
    strHeader<<tr("Æ½¾ù");
    table->setHorizontalHeaderLabels(strHeader);

    // set vertical header
    for(int i=0; i<strSide.size(); i++)
    {
        QString s = strSide.at(i);
        s.insert(s.indexOf(tr("£¨")), '\n');
        s.remove(tr("£¨"));
        s.remove(tr("£©"));
        strSide.replace(i, s);
    }
    table->setRowCount(strSide.size());
    table->setVerticalHeaderLabels(strSide);

    // font and style
    table->setFont(*font);
    table->setFrameShape(QFrame::NoFrame);
    for(int i=0; i<table->rowCount(); i++)
    {
        table->verticalHeaderItem(i)->setFont(*fontSmall);
    }

    table->resizeRowsToContents();
    table->resizeColumnsToContents();
    table->verticalHeader()->setFixedWidth(170);

    // resize columns
    int w = (table->width() - table->verticalHeader()->width()) / table->columnCount();
    for(int i=0; i<table->columnCount(); i++)
    {
        table->setColumnWidth(i, w);
    }

    // resize rows
    int h = (table->height() - table->horizontalHeader()->height()) / table->rowCount();
    for(int i=0; i<table->rowCount(); i++)
    {
        table->setRowHeight(i, h);
    }

    // background color
    QPalette p = table->palette();
    p.setBrush(QPalette::Window, QBrush(QColor(255, 255, 255, 255)));
    table->setPalette(p);

    // clean data
    for(int i=0; i<table->rowCount(); i++)
    {
        for(int j=0; j<table->columnCount(); j++)
        {
            QTableWidgetItem* item = table->item(i, j);
            if(item != 0)
            {
                item->setData(Qt::EditRole, tr(""));
                item->setTextColor(Qt::black);
                item->setBackgroundColor(Qt::white);
            }
        }
    }
}
