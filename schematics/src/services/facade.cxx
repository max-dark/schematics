#include "facade.hxx"

#include <QCommandLineOption>
#include <QCommandLineParser>

#include <QDir>

#include <QDebug>

#include <services/database.hxx>

namespace Schematics::Service
{
Facade::Facade(QObject *parent)
    : QObject(parent)
    , Application{}
    {};

Storage *Facade::storage()
{
    return database;
}

void Facade::parseArguments(const QStringList &argv, const QString &defaultPath, const QString &defaultFile)
{
    QCommandLineOption config{
        QStringList{} << "C" << "config",
        "path to config", "path",
        defaultPath
    };
    QCommandLineOption file{
        QStringList{} << "F" << "file",
        "config file name", "config.db",
        defaultFile
    };
    QCommandLineParser parser{};

    parser.addOption(config);
    parser.addOption(file);

    auto ok = parser.parse(argv);

    if (ok)
    {
        configPath = parser.value(config);
        configFile = parser.value(file);
    }
    else {
        configPath = defaultPath;
        configFile = defaultFile;
    }
    qInfo() << "Parse Args" << ok << databaseFile();
}

QString Facade::databaseFile()
{
    return QDir{configPath}.absoluteFilePath(configFile);
}

void Facade::startStorage()
{
    if (nullptr == database)
    {
        database = new Database{this};
        auto ok = database->open(databaseFile());
        qInfo() << "Start db:" << ok;
        auto valid_db = database->checkStructure();
        qInfo() << "Validate db:" << valid_db;
    }
    else {
        qWarning() << "db already started";
    }
}

bool Facade::getConnectionParams(const QString &name, QString &address, int &interval)
{
    QString str_addr, str_int;
    int tmp_int;
    auto ok = storage()->getValueByName("plc/" + name + "/ip", str_addr);
    ok = ok && storage()->getValueByName("plc/" + name + "/interval", str_int);
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

void Facade::startSawPlc()
{
    QString addr;
    int interval = 0;
    // connect to main PLC

    auto ok = getConnectionParams("sab", addr, interval);
    qDebug() << ok << addr << interval;
}

void Facade::startKdoPlc()
{
    QString addr;
    int interval = 0;
    // connect to secondary PLC

    auto ok = getConnectionParams("kdo", addr, interval);
    qDebug() << ok << addr << interval;

}

OffsetRepository *Facade::offsets() {
    return database;
}

bool Facade::applyCoordById(PositionId id, libschema::Unit value)
{
    // get offset by id
    // get address by id
    // convert to plc format
    // send value to plc
    // send "apply" command
    return false;
}

bool Facade::applyCoordinates(const Coordinates &coords)
{
    // get offsets
    // get coord addr list
    // get addr of "apply" bits
    // for each offset
    //  convert coord to plc varible
    //  send coord variable to plc
    //  send "apply" command
    return false;
}

Facade::~Facade() = default;

} // namespace Schematics::Service
