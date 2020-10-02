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
    auto ok = database->getValueByName("plc/" + name + "/ip", str_addr);
    ok = ok && database->getValueByName("plc/" + name + "/interval", str_int);
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

void Facade::startSabPlc()
{
    QString addr;
    int interval = 0;
    // connect to main PLC
    sab = new Alpha{database, this};
    auto ok = getConnectionParams("sab", addr, interval);
    if (ok && false)
    {
        qInfo() << "try connect to" << addr;
        ok = sab->connect(addr);
        qInfo() << ok << sab->errorMessage();
    }
    qDebug() << ok << addr << interval;
}

void Facade::startKdoPlc()
{
    QString addr;
    int interval = 0;
    // connect to secondary PLC

    kdo = new Machine{this};
    auto ok = getConnectionParams("kdo", addr, interval);
    qDebug() << ok << addr << interval;

    if (ok && false)
    {
        qInfo() << "try connect to" << addr;
        ok = kdo->connect(addr);
        qInfo() << ok << kdo->errorMessage();

        bool parse_ok;
        auto num_addr = TagAddress::from_string("M0:42", parse_ok);
        qInfo() << "parse num" << parse_ok;
        auto bit_addr = BitAddress::from_string("M0:40.0", parse_ok);
        qInfo() << "parse bit" << parse_ok;
        {
            DIntTag num{num_addr};
            BoolTag bit{bit_addr};
            num.set(0);
            bit.set(false);
            auto ok = kdo->writeTag(num) && kdo->writeTag(bit);
            qInfo() << "write" << ok << kdo->errorMessage();
        }

        {
            DIntTag num{num_addr};
            BoolTag bit{bit_addr};
            auto ok = kdo->readTag(num) && kdo->readTag(bit);
            qInfo() << "read" << ok << num.get() << bit.get();
        }
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
