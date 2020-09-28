#include "fasade.hxx"

#include <QCommandLineOption>
#include <QCommandLineParser>

#include <QDir>

#include <QDebug>

#include <services/database.hxx>

namespace Schematics::Service
{
Fasade::Fasade(QObject *parent)
    : QObject(parent)
    , Application{}
    {};

Storage *Fasade::storage()
{
    return database;
}

void Fasade::parseArguments(const QStringList &argv, const QString &defaultPath, const QString &defaultFile)
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

QString Fasade::databaseFile()
{
    return QDir{configPath}.absoluteFilePath(configFile);
}

void Fasade::startDatabase()
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

Fasade::~Fasade() = default;

} // namespace Schematics::Service
