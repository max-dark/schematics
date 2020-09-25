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
    };
} // namespace CoordSystem
