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
    auto db = QSqlDatabase::database(db_name);

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
    QSqlQuery get_value;

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
    QSqlQuery set_query;

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

} // namespace Schematics::Service
