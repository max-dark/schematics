#ifndef COORDINATES_HXX
#define COORDINATES_HXX

#include <schema/schema.hxx>
#include <schema/units.hxx>

#include <coords/positionid.hxx>

#include <array>

namespace Coords
{

using libschema::Unit;
using libschema::Schema;

/**
 * @brief The Coordinates class
 *
 * Содержит список координат оборудования относительно линии
 *
 * Нулевым точкам соответствует
 *  по X: центральная ось линии
 *      координаты для левой и правой части зеркальны
 *  по Y: нижняя плоскость бруса
 */
class Coordinates
{
    using container = std::array<Unit, PositionId::POS_ID_COUNT>;
public:
    using const_iterator = container::const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    Unit byId(PositionId id) const;
    void setById(PositionId id, Unit value);

    /// Заполняет список координат по заданной схеме
    void fill_from(const Schema* schema);
    bool isVerticalMode() const noexcept;
private:
    container coords;
    bool isVertical = false;
};

} // namespace Coords
#endif // COORDINATES_HXX
