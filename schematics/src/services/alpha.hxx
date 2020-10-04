#pragma once

#include <coords/positionid.hxx>
#include <coords/coordinates.hxx>
#include <coords/offset.hxx>

#include <schema/units.hxx>

#include <services/machine.hxx>
#include <services/tags.hxx>

#include <QObject>


namespace Schematics::Service
{

class Alpha: public Machine
{
    Q_OBJECT
public:
    explicit Alpha(Coords::OffsetRepository* offsets,
          QObject* parent = nullptr);

    bool applyCoordById(Coords::PositionId id, libschema::Unit value);
    bool applyCoordinates(const Coords::Coordinates& coords, bool verticalMode);
private:
    void initCoordMap();
    Coords::OffsetRepository* offsetList = nullptr;
    CoordMap coord_map;
};

} // namespace Schematics::Service
