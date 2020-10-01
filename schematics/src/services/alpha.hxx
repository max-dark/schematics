#pragma once

#include <services/machine.hxx>
#include <coords/positionid.hxx>
#include <coords/coordinates.hxx>
#include <coords/offset.hxx>

#include <schema/units.hxx>

#include <QObject>

#include <unordered_map>

namespace Schematics::Service
{

struct CoordWriteAddr
{
    Tag::Area area = Tag::Area::MEMORY;
    int db = 0;
    int byte = 0;
};
struct CoordApplyAddr
{
    Tag::Area area = Tag::Area::MEMORY;
    int db = 0;
    int byte = 0;
    int bit = 0;
};

struct CoordAddress
{
    CoordWriteAddr coord;
    CoordApplyAddr apply;
};

using CoordMap = std::unordered_map<Coords::PositionId, CoordAddress>;

class Alpha: public Machine
{
    Q_OBJECT
public:
    Alpha(Coords::OffsetRepository* offsets,
          QObject* parent = nullptr);

    bool applyCoordById(Coords::PositionId id, libschema::Unit value);
    bool applyCoordinates(const Coords::Coordinates& coords);
private:
    void initCoordMap();
    Coords::OffsetRepository* offsetList = nullptr;
    CoordMap coord_map;
};

} // namespace Schematics::Service
