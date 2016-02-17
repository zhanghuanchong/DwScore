#ifndef DOUBLEDELEGATE_H
#define DOUBLEDELEGATE_H

#include <QStyledItemDelegate>

class DoubleDelegate : public QStyledItemDelegate
{
    Q_OBJECT

private:
    double min;
    double max;

public:
    explicit DoubleDelegate(double min, double max, QObject *parent = 0);

    virtual QWidget* createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    virtual void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};

#endif // DOUBLEDELEGATE_H
