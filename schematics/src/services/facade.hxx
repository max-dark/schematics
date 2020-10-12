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
class AlphaSupport;
class Machine;

class Facade : public QObject, public Application
{
    Q_OBJECT
public:
    explicit Facade(QObject *parent = nullptr);
    ~Facade() override;

    bool applyCoordById(Coords::PositionId id, libschema::Unit value) override;
    bool applyCoordinates(const Coords::Coordinates& coords) override;
    bool getConnectionParams(const QString &name, QString &address, int &interval) override;
    void getConnectionState(bool& alphaPlc, bool& supportPlc) override;

    void resetConnection() override;

    void getAxisInitState(bool& is_started, bool& is_done) override;
    bool startAxisInit() override;

    LabelMap getMotorLabels() override;
    LabelMap getAlarmLabels() override;
    LabelMap getSensorLabels() override;

    BoolMap getMotorState() override;
    BoolMap getAlarmState() override;
    BoolMap getSensorState() override;


    LabelMap getDoorsLabels() override;
    bool toggleDoorById(int doorId) override;

    BoolMap getSupportMotorsState() override;
    BoolMap getSupportAlarmsState() override;

    void parseArguments(const QStringList& argv,
                           const QString& defaultPath,
                           const QString &defaultFile);

    void startStorage();
    void startSabPlc();
    void startKdoPlc();
    void startUpdateCycle();
public slots:
    void updateState();
    void requestStateUpdate();
signals:
    void updateSuccess();
    void updateSabFailure(const QString& error);
    void updateKdoFailure(const QString& error);
private /* methods */:
    QString databaseFile();
    bool isProductionMode() const noexcept;
    bool kdoIsRunning();
private:
    QString configPath;
    QString configFile;

    BooleanMap alarms;
    BooleanMap motors;
    BooleanMap sensors;
    BooleanMap doors;

    bool is_prod_mode = false;
    Database* database = nullptr;
    Alpha* sab = nullptr;
    AlphaSupport* kdo = nullptr;
};

} // namespace Schematics::Service
