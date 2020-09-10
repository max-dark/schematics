#pragma once

#include <cstdint>

namespace libschema
{
using unit_t = int32_t;
using digit_t = int32_t;
using millimeter_t = double;

class Unit
{
public:
    /// create unit from millimeters
    static Unit from_mm(millimeter_t mm);
    static Unit from_units(unit_t units);

    /// get value in units
    unit_t units() const;
    /// get value in millimeters
    millimeter_t to_mm() const;
    /// scale value to digits of analog output
    digit_t to_digits(double per_mm) const;

    bool operator==(Unit rhs) const { return units() == rhs.units(); }
    bool operator!=(Unit rhs) const { return units() != rhs.units(); }

    Unit operator+(const Unit& rhs) const { return Unit{ units() + rhs.units() }; }
    Unit operator-(const Unit& rhs) const { return Unit{ units() - rhs.units() }; }
    Unit operator*(const Unit& rhs) const { return Unit{ units() * rhs.units() }; }
    Unit operator/(const Unit& rhs) const { return Unit{ units() / rhs.units() }; }

    Unit() = default;
    Unit(Unit &&) = default;
    Unit(const Unit &) = default;
    Unit &operator=(Unit &&) = default;
    Unit &operator=(const Unit &) = default;
    ~Unit() = default;

private:
    explicit Unit(unit_t value);
    unit_t value = 0;
};

//namespace units {
//constexpr Unit operator"" _mm(millimeter_t mm) {return Unit::from_mm(mm); }
//constexpr Unit operator"" _unit(unit_t units) {return Unit::from_units(units); }
//}

} // namespace libschema
