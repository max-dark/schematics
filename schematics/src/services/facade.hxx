#pragma once

#include <QObject>

#include <QString>
#include <QStringList>

#include <services/application.hxx>
#include <services/storage.hxx>

#include <coords/offset.hxx>

namespace Schematics::Service
{

using namespace Coords;
class Database;

class Facade : public QObject, public Application
{
    Q_OBJECT
public:
    explicit Facade(QObject *parent = nullptr);
    ~Facade() override;

    Storage *storage() override;
    OffsetRepository* offsets() override;
    void startSawPlc();
    void startKdoPlc();
    bool getConnectionParams(const QString &name, QString &address, int &interval) override;

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
