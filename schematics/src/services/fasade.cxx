#include "fasade.hxx"

#include <QCommandLineOption>
#include <QCommandLineParser>

#include <QDir>

#include <QDebug>

namespace Schematics::Service
{
Fasade::Fasade(QObject *parent) : QObject(parent)
{

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
    qDebug() << ok << databaseFile();
}

QString Fasade::databaseFile()
{
    return QDir{configPath}.absoluteFilePath(configFile);
}

Fasade::~Fasade() = default;

} // namespace Schematics::Service
