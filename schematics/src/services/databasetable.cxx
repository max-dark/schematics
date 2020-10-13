#include "databasetable.hxx"

#include <QSqlField>
#include <QSqlDriver>
#include <QSqlError>
#include <QDebug>

namespace Schematics::Service
{

DatabaseTable::DatabaseTable(const QString &table, const QString& connection, QObject *parent)
    : SettingsTable(parent)
{
    model = new QSqlTableModel{this, QSqlDatabase::database(connection)};
    model->setTable(table);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
}

DatabaseTable::~DatabaseTable()
{}

DatabaseTable* DatabaseTable::setColumnTitle(int idx, const QString &title)
{
    model->setHeaderData(idx, Qt::Horizontal, title);
    return this;
}

QAbstractTableModel *DatabaseTable::table()
{
    return model;
}

void DatabaseTable::saveChanges()
{
    model->submitAll();
}

void DatabaseTable::cancelChanges()
{
    model->revertAll();
}

void DatabaseTable::setFilter(const QString &filter)
{
    if (filter.isEmpty())
    {
        model->setFilter({});
    }
    else
    {
        auto like = filter;
        like.replace("'", ""); //TODO: костыль. Нужно нормальное решение

        model->setFilter("description LIKE '%" + like + "%'");
        // qInfo() << model->lastError().text();
    }
}

} // namespace Schematics::Service
