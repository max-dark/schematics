#pragma once

#include <schema/schema.hxx>

#include <coords/positionid.hxx>

namespace Coords
{
    using libschema::Schema;
    using libschema::Unit;
    /**
     * @brief The SchemeCalculator class
     *  рассчитывает геометрию схемы
     */
    class SchemeCalculator
    {
    public:
        /// calculate sizes and positions by scheme
        void calculate(const Schema* schema);

        Unit fbs1Width() const;
        Unit fbs1Height() const;

        Unit fbs2Align() const;
        Unit fbs2Width() const;
        Unit fbs2Height() const;

        Unit p1Width() const;
        Unit p1Height() const;
        Unit p1Bottom() const;
        Unit p1Top() const;

        Unit p2Width() const;
        Unit p2Height() const;
        Unit p2Bottom() const;
        Unit p2Top() const;
        Unit p2OuterSaw() const;

        Unit p2RollerPos() const;

        Unit p3Width() const;
        Unit p3Height() const;
        Unit p3Bottom() const;
        Unit p3Top() const;

        Unit dwsWidth() const;
        Unit dwsHeight() const;

    private:
        // f1
        Unit fbs1_width;
        Unit fbs1_height;
        // f2
        Unit fbs2_width;
        Unit fbs2_height;
        Unit fbs2_align;
        // p1
        Unit p1_width;
        Unit p1_height;
        Unit p1_bottom;
        Unit p1_top;
        // p2
        Unit p2_width;
        Unit p2_height;
        Unit p2_bottom;
        Unit p2_top;
        Unit p2_outer_saw;
        Unit p2_roller_pos;
        // p2 / rotator
        Unit r2_width;
        Unit r2_height;
        // p3
        Unit p3_width;
        Unit p3_height;
        Unit p3_bottom;
        Unit p3_top;
        // dws
        Unit dws_height;
        Unit dws_width;
    };
} // namespace CoordSystem
