#include "database.hxx"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

namespace Schematics::Service
{

Database::Database(QObject *parent) : QObject(parent)
{

}

Database::~Database() = default;

bool Database::open(const QString &database)
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(database);
    return db.open();
}

bool Database::checkStructure()
{
    auto db = QSqlDatabase::database();

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

bool Database::getConfigByName(const QString &name, QString &value)
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

bool Database::getConnectionParams(const QString &name, QString &address, int &interval)
{
    QString str_addr, str_int;
    int tmp_int;
    auto ok = getConfigByName("plc/" + name + "/ip", str_addr);
    ok = ok && getConfigByName("plc/" + name + "/interval", str_int);
    if (ok)
    {
        tmp_int = str_int.toInt(&ok);
    }
    if(ok)
    {
        address = str_addr;
        interval = tmp_int;
    }
    return ok;
}

} // namespace Schematics::Service
