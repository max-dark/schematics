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
    const auto safe_offset = Unit::from_mm(50);

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

    // P1/P2/R2
    {
        auto width = g.p1Width();
        auto height = g.p2Height();

        auto use_outer_saw = (
                schema->is_pka350_enabled() &&
                schema->is_pa350_enabled()
                );

        // P1
        {
            Unit left_p1, right_p1;
            Unit top_p1, bottom_p1;

            if (schema->is_pa350_enabled())
            {
                auto half = g.p2Width() / two;
                left_p1 = right_p1 = half;
                bottom_p1 = g.p1Bottom();
                top_p1 = g.p1Top();
            }
            else
            {
                // safe positions
                const Unit left_safe_p1 = (width / two) + safe_offset;
                const Unit right_safe_p1 = Unit::from_mm(1000.0);
                const Unit bottom_safe_p1 = Unit::from_units(0) - safe_offset;
                const Unit top_safe_p1 = height + safe_offset;

                // move to safe positions
                left_p1 = left_safe_p1;
                right_p1 = right_safe_p1;
                bottom_p1 = bottom_safe_p1;
                top_p1 = top_safe_p1;
            }

            setById(POS_ID_P1_PA350_ROLLERS_INP_WIDTH, width);

            setById(POS_ID_P1_PA350_LEFT_BLOCK, left_p1);
            setById(POS_ID_P1_PA350_RIGHT_BLOCK, right_p1);

            setById(POS_ID_P1_PA350_BOTTOM_LEFT, bottom_p1);
            setById(POS_ID_P1_PA350_BOTTOM_RIGHT, bottom_p1);

            setById(POS_ID_P1_PA350_TOP_LEFT, top_p1);
            setById(POS_ID_P1_PA350_TOP_RIGHT, top_p1);
        }

        // P2
        {
            Unit left_p2, right_p2;
            Unit top_p2, bottom_p2;

            if (schema->is_pka350_enabled())
            {
                auto half = g.r2Width() / two;
                left_p2 = right_p2 = half;
                bottom_p2 = g.p2Bottom();
                top_p2 = g.p2Top();
            }
            else
            {
                // safe positions
                const Unit left_safe_p2 = (width / two) + safe_offset;
                const Unit right_safe_p2 = Unit::from_mm(1000.0);
                const Unit bottom_safe_p2 = Unit::from_units(0) - safe_offset;
                const Unit top_safe_p2 = height + safe_offset;

                // move to safe positions
                left_p2 = left_safe_p2;
                right_p2 = right_safe_p2;
                bottom_p2 = bottom_safe_p2;
                top_p2 = top_safe_p2;
            }

            setById(POS_ID_P2_PKA350_ROLLERS_INP_WIDTH, width);

            setById(POS_ID_P2_PKA350_LEFT_BLOCK, left_p2);
            setById(POS_ID_P2_PKA350_RIGHT_BLOCK, right_p2);

            setById(POS_ID_P2_PKA350_BOTTOM_LEFT, bottom_p2);
            setById(POS_ID_P2_PKA350_BOTTOM_RIGHT, bottom_p2);

            setById(POS_ID_P2_PKA350_TOP_LEFT, top_p2);
            setById(POS_ID_P2_PKA350_TOP_RIGHT, top_p2);

            setById(POS_ID_P2_PKA350_LEFT_HEIGHT, height / two);
            setById(POS_ID_P2_PKA350_RIGHT_HEIGHT, height / two);

            // внешняя пила 2го профилятора
            {
                Unit saw_pos = use_outer_saw ? g.p2OuterSaw() : Unit::from_mm(100.0);
                setById(POS_ID_P2_PKA350_LEFT_SAW, saw_pos);
                setById(POS_ID_P2_PKA350_RIGHT_SAW, saw_pos);
            }

            setById(POS_ID_P2_PKA350_LEFT_ROLLER, g.p2RollerPos());
            setById(POS_ID_P2_PKA350_RIGHT_ROLLER, g.p2RollerPos());

            setById(POS_ID_P2_PKA350_ROLLERS_OUT_WIDTH, width);
        }

        // seporator
        setById(POS_ID_P2_PKA350_PRESS_HEIGHT, height);

        // second rotator
        setById(POS_ID_P2_PKA350_ROTATOR_HEIGHT, height);
    }

    // P3 / DWS
    {
        auto width = g.p3Width();
        // P3
        {
            Unit left_p3, right_p3;
            Unit top_p3, bottom_p3;

            if (schema->is_pa300_enabled())
            {
                auto half = g.dwsWidth() / two;
                left_p3 = right_p3 = half;
                bottom_p3 = g.p3Bottom();
                top_p3 = g.p3Top();
            }
            else
            {
                // safe positions
                const Unit left_safe_p3 = (width / two) + safe_offset;
                const Unit right_safe_p3 = Unit::from_mm(1000.0);
                const Unit bottom_safe_p3 = Unit::from_units(0) - safe_offset;
                const Unit top_safe_p3 = g.dwsHeight() + safe_offset;

                // move to safe positions
                left_p3 = left_safe_p3;
                right_p3 = right_safe_p3;
                bottom_p3 = bottom_safe_p3;
                top_p3 = top_safe_p3;
            }

            setById(POS_ID_P3_PA300_ROLLERS_INP_WIDTH, width);

            setById(POS_ID_P3_PA300_LEFT_BLOCK, left_p3);
            setById(POS_ID_P3_PA300_RIGHT_BLOCK, right_p3);

            setById(POS_ID_P3_PA300_BOTTOM_LEFT, bottom_p3);
            setById(POS_ID_P3_PA300_BOTTOM_RIGHT, bottom_p3);

            setById(POS_ID_P3_PA300_TOP_LEFT, top_p3);
            setById(POS_ID_P3_PA300_TOP_RIGHT, top_p3);
        }

        // DWS
        {
            setById(POS_ID_DWS350_INP_ROLLERS_WIDTH, width);

            setById(POS_ID_DWS350_PRESS1_HEIGHT, g.dwsHeight());
            setById(POS_ID_DWS350_PRESS2_HEIGHT, g.dwsHeight());
            setById(POS_ID_DWS350_PRESS3_HEIGHT, g.dwsHeight());

            //TODO: it must depend on saw diameter
            setById(POS_ID_DWS350_AXIS_HEIGHT, Unit::from_units(0));

            setById(POS_ID_DWS350_OUT_ROLLERS_WIDTH, width);
        }
    }
}

} // namespace Coords
