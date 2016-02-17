#include <QHeaderView>
#include "rankwidget.h"

RankWidget::RankWidget(QFont* font1, QWidget *parent) :
    QWidget(parent), pageSize(10)
{
    QString styleWhite(tr("color:rgb(255, 255, 255);"));

    labelInfo = new QLabel(this);
    labelInfo->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    labelInfo->setFont(*font1);
    labelInfo->setStyleSheet(styleWhite);
    labelInfo->setGeometry(0, 0, 1024, 130);
    labelInfo->setFont(*font1);

    table = new QTableWidget(this);
    table->setGeometry(100, 180, 824, 422);
    table->setFont(*font1);
    QStringList header;
    header<<tr("排名")<<tr("姓名")<<tr("序号")<<tr("场次")<<tr("分数");
    table->setColumnCount(header.size());
    table->setHorizontalHeaderLabels(header);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setFont(*font1);
    table->setAlternatingRowColors(true);
    int colWidth = (table->width() - 20) / header.size();
    for(int i=0; i<table->columnCount(); i++)
    {
        table->setColumnWidth(i, colWidth);
    }
}

void RankWidget::start(QDomElement *node)
{
    int row = table->rowCount();
    while(row)
    {
        table->removeRow(0);
        row = table->rowCount();
    }
    labelInfo->setText(node->attribute("lunciName").append(tr(" 排名")));
    QDomNodeList list = node->elementsByTagName("player");
    QAbstractItemModel* model = table->model();
    for(int i=0; i<list.size(); i++)
    {
        QDomNode domNode = list.at(i);
        row = table->rowCount();
        table->insertRow(row);
        if(domNode.nodeType() == QDomNode::ElementNode)
        {
            QDomElement element = domNode.toElement();
            model->setData(model->index(row, 0), element.attribute("id"));
            model->setData(model->index(row, 1), element.attribute("name"));
            model->setData(model->index(row, 2), element.attribute("order"));
            model->setData(model->index(row, 3), element.attribute("changci"));
            model->setData(model->index(row, 4), element.attribute("score"));
            table->setRowHeight(row, 38);
            for(int j=0; j<table->columnCount(); j++)
            {
                table->item(row, j)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    index = 0;
    timer = this->startTimer(8000);
}

void RankWidget::timerEvent ( QTimerEvent * )
{
    int canShow = (table->height() - table->horizontalHeader()->height()) / table->rowHeight(0);
    int all = table->rowCount();
    if(all <= canShow)
    {
        return;
    }
    if(index < all - canShow)
    {
        index += canShow;
    }
    else
    {
        index = 0;
    }
    table->scrollTo(table->model()->index(index, 0), QAbstractItemView::PositionAtTop);
}
