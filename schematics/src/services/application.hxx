#pragma once

#include <QString>

#include <coords/coordinates.hxx>
#include <coords/positionid.hxx>
#include <schema/units.hxx>

namespace Schematics::Service
{
struct Application
{
    virtual bool applyCoordById(Coords::PositionId id, libschema::Unit value) = 0;
    virtual bool applyCoordinates(const Coords::Coordinates& coords) = 0;
    virtual bool getConnectionParams(const QString &name, QString &address, int &interval) = 0;
protected:
    virtual ~Application();
};

}
