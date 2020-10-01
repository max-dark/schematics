#include <services/alpha.hxx>
#include <services/machine.hxx>

#include <QDebug>

namespace Schematics::Service
{

struct OffsetWriter: public Coords::OffsetVisitor
{
    OffsetWriter(CoordAddress addr,libschema::Unit value, Machine* self)
    {
        a = addr;
        v = value;
        m = self;
    }
    bool write(const Coords::Offset* o)
    {
        o->accept(*this);
        return ok;
    }
    bool write(const Tag& coord, const BoolTag apply)
    {
        if constexpr(false)
        {
            return m->writeTag(coord) &&
                   m->writeTag(apply);
        }
        else
        {
            qDebug() << "Write coord to" << coord.address()
                     << "Write apply to" << apply.address();
        }
        return false;
    }
    void visit(const Coords::UnitOffset& offset) override
    {
        DIntTag coord{
            a.coord.area, a.coord.byte, a.coord.db
        };
        BoolTag apply{
            a.apply.area,
            a.apply.byte, a.apply.bit,
            a.apply.db
        };
        coord.set(v.units() + offset.offset);
        apply.set(true);

        ok = write(coord, apply);
    }
    void visit(const Coords::DigitOffset& offset) override
    {
        IntTag coord{
            a.coord.area, a.coord.byte, a.coord.db
        };
        BoolTag apply{
            a.apply.area,
            a.apply.byte, a.apply.bit,
            a.apply.db
        };
        coord.set(v.to_digits(offset.per_mm) + offset.offset);
        apply.set(true);

        ok = write(coord, apply);
    }
private:
    CoordAddress a;
    libschema::Unit v;
    Machine* m = nullptr;
    bool ok = false;
};

Alpha::Alpha(Coords::OffsetRepository *offsets,
             QObject *parent)
    : Machine(parent)
    , offsetList{offsets}
{
    initCoordMap();
}

bool Alpha::applyCoordById(Coords::PositionId id, libschema::Unit value)
{
    auto ok = false;
    auto offsets = offsetList->offsets();
    auto o = offsets.find(id);
    if (o != offsets.end())
    {
        //if (false)
        {
            CoordAddress a{};
            ok = OffsetWriter{a, value, this}.write(o->second);
        }
    }
    offsetList->clear(offsets);
    return ok;
}

bool Alpha::applyCoordinates(const Coords::Coordinates &coords)
{
    auto offsets = offsetList->offsets();
    for(const auto& o: offsets)
    {
        auto coordValue = coords.byId(o.first);
        //if (false)
        {
            CoordAddress a{};
            OffsetWriter{a, coordValue, this}.write(o.second);
        }
    }
    offsetList->clear(offsets);
    return false;
}

void Alpha::initCoordMap()
{
    using namespace Coords;
    constexpr auto area = Tag::Area::MEMORY;
    if (!coord_map.empty()) return;
    coord_map[POS_ID_FBS1_LEFT_BLOCK] = {
        .coord = {.area = area, .db = 0, .byte = 0},
        .apply = {.area = area, .db = 0, .byte = 0, .bit = 0}};
}

} // namespace Schematics::Service
