#pragma once

#include <QObject>

#include <services/storage.hxx>

namespace Schematics::Service
{

class Database : public QObject, public Storage
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database() override;
    bool open(const QString& database);
    bool checkStructure();
signals:

    // Storage interface
public:
    bool getConnectionParams(const QString &name, QString &address, int &interval) override;
};

} // namespace Schematics::Service
