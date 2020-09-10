#include <schema/units.hxx>
#include <cmath>

namespace libschema
{
    Unit::Unit(unit_t value):value{value} {}

    Unit Unit::from_mm(millimeter_t mm)
    {
        return Unit{static_cast<unit_t>(round(mm * 100.0))};
    }

    Unit Unit::from_units(unit_t units)
    {
        return Unit{units};
    }

    unit_t Unit::units() const { return value; }

    millimeter_t Unit::to_mm() const
    {
        return static_cast<millimeter_t>(value) / 100.0;
    }

    digit_t Unit::to_digits(double per_mm) const
    {
        return static_cast<digit_t>(round(to_mm() * per_mm));
    }
} // namespace libschema
