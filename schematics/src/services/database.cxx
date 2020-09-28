#include "database.hxx"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

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
        auto tables = db.tables();
        ok = ok && (! tables.empty());
        ok = ok && (tables.indexOf("config") != -1);
        ok = ok && (tables.indexOf("offsets") != -1);
        ok = ok && (tables.indexOf("speeds") != -1);
        ok = ok && (tables.indexOf("delays") != -1);
        ok = ok && (tables.indexOf("alarms") != -1);
        ok = ok && (tables.indexOf("sensors") != -1);
        ok = ok && (tables.indexOf("motors") != -1);
    }
    return ok;
}

bool Database::getValueByName(const QString &name, QString &value)
{
    QSqlQuery get_value;

    auto ok = get_value.prepare("select value from config where name = :var_name");
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

bool Database::setValueByName(const QString &name, const QString &value) {
    return false;
}

} // namespace Schematics::Service
