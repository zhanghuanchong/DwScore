#ifndef RANKWIDGET_H
#define RANKWIDGET_H

#include <QLabel>
#include <QTableWidget>
#include <QDomElement>

class RankWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RankWidget(QFont* font1, QWidget *parent = 0);
    void start(QDomElement* node);

private:
    QLabel* labelInfo;
    QTableWidget* table;
    int pageSize;
    int index;
    int timer;

protected:
    virtual void timerEvent ( QTimerEvent * event );
};

#endif // RANKWIDGET_H
