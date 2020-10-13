#pragma once

#include <services/settingstable.hxx>

#include <QSqlTableModel>
#include <QSqlDatabase>

namespace Schematics::Service
{
class DatabaseTable: public SettingsTable
{
    Q_OBJECT
public:
    explicit DatabaseTable(const QString& table, const QString &connection, QObject* parent = nullptr);
    ~DatabaseTable() override;

    // SettingsTable interface
public:
    QAbstractTableModel *table() override;
    void saveChanges() override;
    void cancelChanges() override;
    void setFilter(const QString &filter) override;
private:
    QSqlTableModel* model;
};

} // namespace Schematics::Service
