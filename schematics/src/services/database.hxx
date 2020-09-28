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
    bool getValueByName(const QString& name, QString& value) override;
    bool setValueByName(const QString &name, const QString &value) override;
private:
    QString db_name;
};

} // namespace Schematics::Service
