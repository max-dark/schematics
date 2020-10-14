#pragma once

#include <QObject>
#include <QString>
#include <QAbstractTableModel>

namespace Schematics::Service
{
class SettingsTable : public QObject
{
    Q_OBJECT
public:
    explicit SettingsTable(QObject *parent = nullptr);
    ~SettingsTable() override;

    virtual QAbstractTableModel* table() = 0;
    virtual void saveChanges() = 0;
    virtual void cancelChanges() = 0;
    virtual void setFilter(const QString& filter) = 0;
};

} // namespace Schematics::Service

