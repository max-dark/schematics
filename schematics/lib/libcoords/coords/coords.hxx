#pragma once

#include <schema/schema.hxx>

#include <coords/positionid.hxx>

namespace Coords
{
    using Schema = libschema::Schema;
    class System
    {
    public:
        void calculate(const Schema* schema);
    };
} // namespace CoordSystem
