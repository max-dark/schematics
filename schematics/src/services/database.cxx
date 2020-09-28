#include "database.hxx"

#include <QSqlDatabase>

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

} // namespace Schematics::Service
