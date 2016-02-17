#ifndef INITWIZARD_H
#define INITWIZARD_H

#include <QWizard>
#include <QSqlDatabase>
#include "ui_initwizard.h"

class InitWizard : public QWizard, public Ui_InitWizard
{
    Q_OBJECT

public:
    explicit InitWizard(QSqlDatabase*, QWidget *parent = 0);
    virtual bool validateCurrentPage ();

private slots:
    void insertRecord();
    void delRecord();
    void checkConfig(QString str);
    void submit();

private:
    QSqlDatabase* db;
    void initTableWidget(QTableWidget* table, QString sql);
    void submitTableWidget(QTableWidget* table, QString sql, QString tableName);
};

#endif // INITWIZARD_H
