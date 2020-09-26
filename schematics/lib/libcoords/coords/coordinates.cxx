#include "coordinates.hxx"

#include <coords/geometry.hxx>

namespace Coords
{

Coordinates::const_iterator Coordinates::begin() const
{
    return coords.begin();
}

Coordinates::const_iterator Coordinates::end() const
{
    return coords.end();
}

libschema::Unit Coordinates::byId(PositionId id) const
{
    return coords.at(id);
}

void Coordinates::setById(PositionId id, libschema::Unit value)
{
    coords.at(id) = value;
}

void Coordinates::fill_from(const libschema::Schema *schema)
{
    Geometry g;
    g.calculate(schema);
}

} // namespace Coords
