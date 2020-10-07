#include "facade.hxx"

#include <QCommandLineOption>
#include <QCommandLineParser>

#include <QDir>

#include <QDebug>

#include <services/database.hxx>
#include <services/alpha.hxx>

namespace Schematics::Service
{
Facade::Facade(QObject *parent)
    : QObject(parent)
    , Application{}
    {};

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
    QCommandLineOption mode{
        QStringList{} << "M" << "mode",
        "running mode(production|testing)", "mode",
        "testing"
    };
    QCommandLineParser parser{};

    parser.addOption(config);
    parser.addOption(file);
    parser.addOption(mode);

    auto ok = parser.parse(argv);

    if (ok)
    {
        configPath = parser.value(config);
        configFile = parser.value(file);
        is_prod_mode = "production" == parser.value(mode);
    }
    else {
        configPath = defaultPath;
        configFile = defaultFile;
        is_prod_mode = false;
    }
    qInfo() << "Parse args ok?" << ok;
    qInfo() << "Using database" << databaseFile();
    qInfo() << "Runing mode:" << (isProductionMode() ? "production" : "testing");
}

QString Facade::databaseFile()
{
    return QDir{configPath}.absoluteFilePath(configFile);
}

bool Facade::isProductionMode() const noexcept
{
    return is_prod_mode;
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
    auto ok = database->getValueByName("plc/" + name + "/ip", str_addr);
    ok = ok && database->getValueByName("plc/" + name + "/interval", str_int);
    if (ok)
    {
        tmp_int = str_int.toInt(&ok);
        if(ok)
        {
            address = str_addr;
            interval = tmp_int;
        }
    }
    return ok;
}

void Facade::getConnectionState(bool &sab, bool &kdo)
{
    sab = this->sab->connected() && (!this->sab->hasError());
    kdo = this->kdo->connected() && (!this->kdo->hasError());
}

void Facade::startSabPlc()
{
    QString addr;
    int interval = 0;
    // connect to main PLC
    sab = new Alpha{database, this};
    auto ok = getConnectionParams("sab", addr, interval);
    if (ok && isProductionMode())
    {
        ok = sab->connect(addr);
        qInfo() << "try connect to" << addr
                << ok << sab->errorMessage();
        ok = sab->updateCache();
        qInfo() << "Try update:" << sab->errorMessage()
                << sab->readCachedBit({.area = Tag::Area::MEMORY, .byte = 30, .bit = 1});
    }
    qDebug() << ok << addr << interval;
}

void Facade::startKdoPlc()
{
    QString addr;
    int interval = 0;
    // connect to secondary PLC

    kdo = new Machine{this};
    {
        // CPU 313C IO
        kdo->registerCacheArea({.area = Tag::Area::INPUT , .byte = 124}, 3);
        kdo->registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 124}, 2);
        // CPU 313C retentive memory:
        // конфигурация, флаги состояния
        // %MB10 занят под "Clock Memory"
        kdo->registerCacheArea({.area = Tag::Area::MEMORY, .byte = 0}, 16);
        // DI16xDC24V
        kdo->registerCacheArea({.area = Tag::Area::INPUT , .byte = 0}, 2);
    }
    auto ok = getConnectionParams("kdo", addr, interval);
    qDebug() << ok << addr << interval;

    if (ok && isProductionMode())
    {
        qInfo() << "try connect to" << addr;
        ok = kdo->connect(addr);
        qInfo() << ok << kdo->errorMessage();
    }
}

bool Facade::applyCoordById(PositionId id, libschema::Unit value)
{
    // get offset by id
    // get address by id
    // convert to plc format
    // send value to plc
    // send "apply" command
    return sab->applyCoordById(id, value);
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
    return sab->applyCoordinates(coords);
}

Facade::~Facade() = default;

} // namespace Schematics::Service
