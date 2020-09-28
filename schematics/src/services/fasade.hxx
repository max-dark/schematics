#pragma once

#include <QObject>

#include <QString>
#include <QStringList>

namespace Schematics::Service
{

class Database;

class Fasade : public QObject
{
    Q_OBJECT
public:
    explicit Fasade(QObject *parent = nullptr);
    ~Fasade() override;

    void parseArguments(const QStringList& argv,
                           const QString& defaultPath,
                           const QString &defaultFile);
    QString databaseFile();

    void startDatabase();
signals:
private:
    QString configPath;
    QString configFile;
    Database* database = nullptr;
};

} // namespace Schematics::Service
