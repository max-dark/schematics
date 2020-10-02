#include "database.hxx"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSet>

namespace Schematics::Service
{

Database::Database(QObject *parent)
    : QObject(parent)
    , Storage()
    , OffsetRepository{}
    , db_name{"configuration"}
{}

Database::~Database() = default;

bool Database::open(const QString &database)
{
    auto db = QSqlDatabase::addDatabase("QSQLITE", db_name);

    db.setDatabaseName(database);
    return db.open();
}

bool Database::checkStructure()
{
    auto db = database();

    auto ok = db.isValid() && db.isOpen();
    if (ok)
    {
        auto const table_list = db.tables();
        auto const tables = QSet(table_list.begin(), table_list.end());
        auto table_exists = [&tables](const QString& name)
        {
            return tables.end() != tables.find(name);
        };
        ok = ok && (! tables.empty());
        ok = ok && table_exists("config");
        ok = ok && table_exists("offsets");
        ok = ok && table_exists("speeds");
        ok = ok && table_exists("delays");
        ok = ok && table_exists("alarms");
        ok = ok && table_exists("sensors");
        ok = ok && table_exists("motors");
    }
    return ok;
}

bool Database::getValueByName(const QString &name, QString &value)
{
    QSqlQuery get_value{database()};

    auto ok = get_value.prepare(
        "select value from config where name = :var_name"
        );
    if (ok)
    {
        get_value.bindValue(":var_name", name);
        ok = get_value.exec();
        ok = ok && get_value.next();
        if (ok)
        {
            value = get_value.value("value").toString();
        }
    }
    return ok;
}

bool Database::setValueByName(const QString &name, const QString &value)
{
    QSqlQuery set_query{database()};

    auto ok = set_query.prepare(
        "update config set value = :value where name = :name"
        );
    if (ok)
    {
        set_query.bindValue(":name", name);
        set_query.bindValue(":value", value);
        ok = set_query.exec();
    }
    return ok;
}

bool Database::try_save(PositionId id, OffsetType type, int32_t offset, double per_mm)
{
    QSqlQuery save{database()};
    auto ok = save.prepare(
            "update offsets"
            "   set offset = :offset, type = :type, per_mm = :per_mm"
            "   where id = :id");
    if (ok)
    {
        save.bindValue(":offset", offset);
        save.bindValue(":type", type);
        save.bindValue(":per_mm", per_mm);
        save.bindValue(":id", id);
        ok = save.exec();
    }
    return ok;
}

bool Database::try_load(OffsetList &list)
{
    QSqlQuery load{database()};
    OffsetList tmp;
    auto ok = load.prepare("select id, type, offset, per_mm from offsets");

    if (ok)
    {
        ok = load.exec();
        while (ok && load.next())
        {
            auto id = static_cast<PositionId>(load.value("id").toInt());
            auto type_id = load.value("type").toInt();
            auto type =  (type_id >=0 && type_id <=2)
                            ? OffsetType(type_id)
                            : BROKEN_OFFSET;
            auto offset = load.value("offset").toInt();
            auto  per_mm = load.value("per_mm").toDouble();
            Offset* ptr = nullptr;
            switch (type) {
                case Coords::DIGIT_OFFSET:
                    ptr = createDigit(offset, per_mm);
                    break;
                case UNIT_OFFSET:
                    ptr = createUnit(offset);
                    break;
                case BROKEN_OFFSET:
                    ptr = createBroken(offset, per_mm);
                    break;
            }
            tmp[id] = ptr;
        }
        if (ok)
        {
            std::swap(tmp, list);
        }
    }

    clear(tmp);
    return ok;
}

QSqlDatabase Database::database()
{
    return QSqlDatabase::database(db_name);
}

} // namespace Schematics::Service
