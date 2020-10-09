#pragma once

#include <QObject>

#include <services/storage.hxx>

#include <coords/offset.hxx>

#include <QSqlDatabase>

namespace Schematics::Service
{
    using namespace Coords;
class Database : public QObject
        , public Storage
        , public OffsetRepository
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

    BooleanMap getBooleansByName(const QString &name) override;
    NumberMap getNumbersByName(const QString &name) override;
protected:
    bool try_save(PositionId id, OffsetType type, int32_t offset, double per_mm) override;
    bool try_load(OffsetList &list) override;
private:
    QSqlDatabase database();
    QString db_name;
};

} // namespace Schematics::Service
