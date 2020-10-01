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

    // FBS1
    {
        coord_map[POS_ID_FBS1_LEFT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 66},
            .apply = {.area = area, .db = 0, .byte = 31, .bit = 0}};
        coord_map[POS_ID_FBS1_RIGHT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 70},
            .apply = {.area = area, .db = 0, .byte = 31, .bit = 1}};
        coord_map[POS_ID_FBS1_PRESS1_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 700},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 0}};
        coord_map[POS_ID_FBS1_PRESS2_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 702},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 1}};
        coord_map[POS_ID_FBS1_ROTATOR_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 714},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 7}};
        coord_map[POS_ID_FBS1_ROTATOR_INP_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 710},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 5}};
        coord_map[POS_ID_FBS1_ROTATOR_OUT_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 712},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 6}};
    }

    // FBS2
    {
        coord_map[POS_ID_FBS2_LEFT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 74},
            .apply = {.area = area, .db = 0, .byte = 31, .bit = 5}};
        coord_map[POS_ID_FBS2_RIGHT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 78},
            .apply = {.area = area, .db = 0, .byte = 31, .bit = 6}};
        coord_map[POS_ID_FBS2_ALIGN1] = {
            .coord = {.area = area, .db = 0, .byte = 82},
            .apply = {.area = area, .db = 0, .byte = 32, .bit = 2}};
        coord_map[POS_ID_FBS2_ALIGN2] = {
            .coord = {.area = area, .db = 0, .byte = 86},
            .apply = {.area = area, .db = 0, .byte = 32, .bit = 3}};
        coord_map[POS_ID_FBS2_ALIGN3] = {
            .coord = {.area = area, .db = 0, .byte = 90},
            .apply = {.area = area, .db = 0, .byte = 32, .bit = 4}};
        coord_map[POS_ID_FBS2_PRESS1_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 704},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 2}};
        coord_map[POS_ID_FBS2_PRESS2_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 706},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 3}};
        coord_map[POS_ID_FBS2_PRESS3_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 708},
            .apply = {.area = area, .db = 0, .byte = 45, .bit = 4}};
    }

    // P1 / PA350
    {
        coord_map[POS_ID_P1_PA350_LEFT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 150},
            .apply = {.area = area, .db = 0, .byte = 33, .bit = 1}};
        coord_map[POS_ID_P1_PA350_RIGHT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 154},
            .apply = {.area = area, .db = 0, .byte = 33, .bit = 2}};
        coord_map[POS_ID_P1_PA350_TOP_LEFT] = {
            .coord = {.area = area, .db = 0, .byte = 166},
            .apply = {.area = area, .db = 0, .byte = 34, .bit = 0}};
        coord_map[POS_ID_P1_PA350_TOP_RIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 170},
            .apply = {.area = area, .db = 0, .byte = 34, .bit = 1}};
        coord_map[POS_ID_P1_PA350_BOTTOM_LEFT] = {
            .coord = {.area = area, .db = 0, .byte = 158},
            .apply = {.area = area, .db = 0, .byte = 33, .bit = 6}};
        coord_map[POS_ID_P1_PA350_BOTTOM_RIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 162},
            .apply = {.area = area, .db = 0, .byte = 33, .bit = 7}};
        coord_map[POS_ID_P1_PA350_ROLLERS_INP_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 716},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 0}};
    }
}

} // namespace Schematics::Service
