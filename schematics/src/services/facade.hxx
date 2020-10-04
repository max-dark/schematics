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
class Alpha;
class Machine;

class Facade : public QObject, public Application
{
    Q_OBJECT
public:
    explicit Facade(QObject *parent = nullptr);
    ~Facade() override;

    bool applyCoordById(Coords::PositionId id, libschema::Unit value) override;
    bool applyCoordinates(const Coords::Coordinates& coords, bool verticalMode) override;
    bool getConnectionParams(const QString &name, QString &address, int &interval) override;

    void parseArguments(const QStringList& argv,
                           const QString& defaultPath,
                           const QString &defaultFile);

    void startStorage();
    void startSabPlc();
    void startKdoPlc();
signals:
private /* methods */:
    QString databaseFile();
private:
    QString configPath;
    QString configFile;
    Database* database = nullptr;
    Alpha* sab = nullptr;
    Machine* kdo = nullptr;
};

} // namespace Schematics::Service
