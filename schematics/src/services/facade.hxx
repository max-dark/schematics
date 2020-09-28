#pragma once

#include <QObject>

#include <QString>
#include <QStringList>

#include <services/application.hxx>
#include <services/storage.hxx>

namespace Schematics::Service
{

class Database;

class Facade : public QObject, public Application
{
    Q_OBJECT
public:
    explicit Facade(QObject *parent = nullptr);
    ~Facade() override;

    Storage *storage() override;


    void parseArguments(const QStringList& argv,
                           const QString& defaultPath,
                           const QString &defaultFile);

    void startStorage();
signals:
private /* methods */:
    QString databaseFile();
private:
    QString configPath;
    QString configFile;
    Database* database = nullptr;
};

} // namespace Schematics::Service
