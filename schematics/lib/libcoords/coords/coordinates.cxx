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

    const auto two = Unit::from_units(2);

    // FBS1
    {
        auto height = g.fbs1Height();
        auto x_half = g.fbs1Width() / two;

        setById(POS_ID_FBS1_PRESS1_HEIGHT, height);
        setById(POS_ID_FBS1_PRESS2_HEIGHT, height);

        setById(POS_ID_FBS1_LEFT_BLOCK, x_half);
        setById(POS_ID_FBS1_RIGHT_BLOCK, x_half);

        setById(POS_ID_FBS1_ROTATOR_HEIGHT, height);
        setById(POS_ID_FBS1_ROTATOR_INP_WIDTH, height);
        setById(POS_ID_FBS1_ROTATOR_OUT_WIDTH, height);
    }

    // FBS2
    {
        auto height = g.fbs2Height();
        auto x_half = g.fbs2Width() / two;
        auto align  = g.fbs2Align();

        setById(POS_ID_FBS2_ALIGN1, align);
        setById(POS_ID_FBS2_ALIGN2, align);
        setById(POS_ID_FBS2_ALIGN3, align);

        setById(POS_ID_FBS2_PRESS1_HEIGHT, height);
        setById(POS_ID_FBS2_PRESS2_HEIGHT, height);
        setById(POS_ID_FBS2_PRESS3_HEIGHT, height);

        setById(POS_ID_FBS2_LEFT_BLOCK, x_half);
        setById(POS_ID_FBS2_RIGHT_BLOCK, x_half);
    }
}

} // namespace Coords
