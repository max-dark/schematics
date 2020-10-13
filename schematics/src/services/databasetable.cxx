#include "databasetable.hxx"

#include <QSqlField>
#include <QSqlDriver>
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
        model->setFilter("");
    }
    else
    {
        auto db = model->database();
        QSqlField txt("", QVariant::String);
        txt.setValue(filter);
        auto like = db.driver()->formatValue(txt);
        qDebug() << "raw" << filter << "formated" << like;
        model->setFilter("description LIKE \"%" + filter + "%\"");
    }
}

} // namespace Schematics::Service
