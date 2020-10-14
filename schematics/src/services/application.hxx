#pragma once

#include <QString>
#include <map>

#include <coords/coordinates.hxx>
#include <coords/positionid.hxx>
#include <schema/units.hxx>

#include <services/settingstable.hxx>

namespace Schematics::Service
{
using LabelMap = std::map<int, QString>;
using BoolMap = std::map<int, bool>;
struct Application
{
    virtual bool applyCoordById(Coords::PositionId id, libschema::Unit value) = 0;
    virtual bool applyCoordinates(const Coords::Coordinates& coords) = 0;
    virtual bool getConnectionParams(const QString &name, QString &address, int &interval) = 0;
    virtual void getConnectionState(bool& alphaPlc, bool& supportPlc) = 0;

    virtual SettingsTable* coordsTable() = 0;
    virtual SettingsTable* delaysTable() = 0;
    virtual SettingsTable* speedsTable() = 0;

    virtual void resetConnection() = 0;

    virtual void getAxisInitState(bool& is_started, bool& is_done) = 0;
    virtual bool startAxisInit() = 0;

    virtual LabelMap getMotorLabels() = 0;
    virtual LabelMap getAlarmLabels() = 0;
    virtual LabelMap getSensorLabels() = 0;

    virtual BoolMap getMotorState() = 0;
    virtual BoolMap getAlarmState() = 0;
    virtual BoolMap getSensorState() = 0;

    virtual LabelMap getDoorsLabels() = 0;
    virtual bool toggleDoorById(int doorId) = 0;

    virtual BoolMap getSupportMotorsState() = 0;
    virtual BoolMap getSupportAlarmsState() = 0;
protected:
    virtual ~Application();
};

}
