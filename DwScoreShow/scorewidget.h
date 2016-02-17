#ifndef SCOREWIDGET_H
#define SCOREWIDGET_H

#include <QLabel>
#include <QTableWidget>

class ScoreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScoreWidget(QFont* font, QWidget *parent = 0);
    void setTable(int, QStringList, QFont*, QFont*);

    QLabel* labelInfo;
    QTableWidget* table;
    QLabel* labelTip;

};

#endif // SCOREWIDGET_H
