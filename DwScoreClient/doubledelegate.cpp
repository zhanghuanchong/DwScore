#include <QDoubleSpinBox>
#include "doubledelegate.h"

DoubleDelegate::DoubleDelegate(double _min, double _max, QObject *parent) :
    QStyledItemDelegate(parent), min(_min), max(_max)
{
}

QWidget* DoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    editor->setMinimum(this->min);
    editor->setMaximum(this->max);

    return editor;
}

void DoubleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox* spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
