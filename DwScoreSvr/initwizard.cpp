#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlError>
#include "initwizard.h"

int getConfigInt(QString);
QString getConfigStr(QString);
bool setConfig(QString, QString);

InitWizard::InitWizard(QSqlDatabase* _db, QWidget *parent) :
    QWizard(parent), db(_db)
{
    setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(submit()));

    editSystemName->setText(getConfigStr("systemName"));
    editSystemNameE->setText(getConfigStr("systemNameE"));
    editOrganizers->setText(getConfigStr("organizers"));
    editOrganizersE->setText(getConfigStr("organizersE"));
    editContractors->setText(getConfigStr("contractors"));
    editContractorsE->setText(getConfigStr("contractorsE"));
    editTime->setText(getConfigStr("time"));
    editTimeE->setText(getConfigStr("timeE"));
    editPlace->setText(getConfigStr("place"));
    editPlaceE->setText(getConfigStr("placeE"));
    editAdmin->setText(getConfigStr("adminPwd"));
    editShow->setText(getConfigStr("showPwd"));
    connect(editSystemName, SIGNAL(textChanged(QString)), this, SLOT(checkConfig(QString)));
    connect(editAdmin, SIGNAL(textChanged(QString)), this, SLOT(checkConfig(QString)));
    connect(editShow, SIGNAL(textChanged(QString)), this, SLOT(checkConfig(QString)));

    initTableWidget(tableJury, "select [name],[pwd] from [jury]");
    initTableWidget(tablePlayer, "select [name],[title],[changci],[order] from [player]");
    initTableWidget(tableLunci, "select [name],[changciCount],[upgradeCount],[order] from [lunci]");
    initTableWidget(tableHuanjie, "select [name],[lunci],[totalTime],[bellTime],[endTime],[min],[max],[order] from [huanjie]");

    connect(this->btnJuryAdd, SIGNAL(clicked()), this, SLOT(insertRecord()));
    connect(this->btnPlayerAdd, SIGNAL(clicked()), this, SLOT(insertRecord()));
    connect(this->btnLunciAdd, SIGNAL(clicked()), this, SLOT(insertRecord()));
    connect(this->btnHuanjieAdd, SIGNAL(clicked()), this, SLOT(insertRecord()));

    connect(this->btnJuryDel, SIGNAL(clicked()), this, SLOT(delRecord()));
    connect(this->btnPlayerDel, SIGNAL(clicked()), this, SLOT(delRecord()));
    connect(this->btnLunciDel, SIGNAL(clicked()), this, SLOT(delRecord()));
    connect(this->btnHuanjieDel, SIGNAL(clicked()), this, SLOT(delRecord()));
}

void InitWizard::initTableWidget(QTableWidget *table, QString sql)
{
    QSqlQuery query = db->exec(sql);
    QAbstractItemModel* model = table->model();
    while(query.next())
    {
        int row = table->rowCount();
        table->insertRow(row);
        for(int i=0;i < query.record().count();i++)
        {
            model->setData(model->index(row, i), query.value(i).toString());
        }
    }
    table->resizeColumnsToContents();
}

void InitWizard::checkConfig(QString str)
{
    if(str.trimmed() == "")
    {
        button(QWizard::NextButton)->setEnabled(false);
    }
    else if(editAdmin->text().trimmed() != ""
            && editShow->text().trimmed() != ""
            && editSystemName->text().trimmed() != "")
    {
        button(QWizard::NextButton)->setEnabled(true);
    }
}

void InitWizard::insertRecord()
{
    QTableWidget* table = this->currentPage()->findChild<QTableWidget*>();
    table->insertRow(table->rowCount());
    if(table->rowCount() > 0)
    {
        button(QWizard::NextButton)->setEnabled(true);
    }
}

void InitWizard::delRecord()
{
    QTableWidget* table = this->currentPage()->findChild<QTableWidget*>();
    if(table->selectedRanges().count() == 0)
    {
        QMessageBox::warning(this, tr("错误"), tr("请选择要删除的行。"));
    }
    else
    {
        table->removeRow(table->selectedRanges().first().bottomRow());
    }
    if(table->rowCount() == 0)
    {
        button(QWizard::NextButton)->setEnabled(false);
    }
}

bool InitWizard::validateCurrentPage()
{
    QTableWidget* table = this->currentPage()->findChild<QTableWidget*>();
    if(table)
    {
        QAbstractItemModel* model = table->model();
        for(int i=0; i<table->rowCount(); i++)
        {
            for(int j=0; j<table->columnCount(); j++)
            {
                if(model->data(model->index(i, j)).toString().trimmed() == "")
                {
                    QMessageBox::warning(this, tr("错误"), tr("表格中第%1行第%2列存在空值！").arg(i+1).arg(j+1));
                    model->setData(model->index(i, j), tr(""));
                    table->editItem(table->item(i, j));
                    return false;
                }
            }
        }
    }
    return true;
}

void InitWizard::submit()
{
    setConfig("systemName", editSystemName->text().trimmed());
    setConfig("systemNameE", editSystemNameE->text().trimmed());
    setConfig("organizers", editOrganizers->text().trimmed());
    setConfig("organizersE", editOrganizersE->toPlainText().trimmed());
    setConfig("contractors", editContractors->text().trimmed());
    setConfig("contractorsE", editContractorsE->text().trimmed());
    setConfig("time", editTime->text().trimmed());
    setConfig("timeE", editTimeE->text().trimmed());
    setConfig("place", editPlace->text().trimmed());
    setConfig("placeE", editPlaceE->text().trimmed());
    setConfig("adminPwd", editAdmin->text().trimmed());
    setConfig("showPwd", editShow->text().trimmed());
    setConfig("currentOrder", "1");
    setConfig("currentLunci", "1");
    setConfig("currentChangci", "1");
    setConfig("currentHuanjie", "1");

    submitTableWidget(tableJury, "insert into [jury]([name], [pwd])", "jury");
    submitTableWidget(tablePlayer, "insert into [player]([name], [title], [changci], [order])", "player");
    submitTableWidget(tableLunci, "insert into [lunci]([name], [changciCount], [upgradeCount], [order])", "lunci");
    submitTableWidget(tableHuanjie, "insert into [huanjie]([name], [lunci], [totalTime], [bellTime], [endTime], [min], [max], [order])", "huanjie");

    db->exec("delete from [score]");
    db->exec("vacuum");
}

void InitWizard::submitTableWidget(QTableWidget *table, QString sql, QString tableName)
{
    db->exec(tr("delete from [%1]").arg(tableName));
    QString str;
    QAbstractItemModel* model = table->model();
    for(int i=0; i<table->rowCount(); i++)
    {
        str = sql;
        str.append(" values(");
        for(int j=0; j<table->columnCount(); j++)
        {
            str.append(tr("'%1',").arg(model->data(model->index(i, j)).toString()));
        }
        str = str.left(str.length() - 1);
        str.append(")");
        db->exec(str);
    }
}
