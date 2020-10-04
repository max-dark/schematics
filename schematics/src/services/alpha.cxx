#include <services/alpha.hxx>
#include <services/machine.hxx>

#include <QDebug>

namespace Schematics::Service
{
namespace Const
{
static const BitAddress axisStartInit{ .area = Tag::Area::MEMORY, .db = 0, .byte =30, .bit = 0 };
static const BitAddress axisInitDone{ .area = Tag::Area::MEMORY, .db = 0, .byte = 30, .bit = 1 };

/// %M2044.6 отключить 2е поворотное устройство
static const BitAddress disableRotator{ .area = Tag::Area::MEMORY, .db = 0, .byte = 2044, .bit = 6 };
}

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
        qDebug() << "value mm" << v.to_mm();
        o->accept(*this);
        return ok;
    }
    bool write(const Tag& coord, const BoolTag& apply)
    {
        {
            qDebug() << "Write coord to" << (coord.address())
                     << "Write apply to" << (apply.address() >> 3) << (apply.address() & 3);
        }
        //if constexpr(false) // TODO: remove it
        {
            return m->writeTag(coord) &&
                   m->writeTag(apply);
        }
        return false;
    }
    void visit(const Coords::UnitOffset& offset) override
    {
        DIntTag coord{a.coord};
        BoolTag apply{a.apply};
        coord.set(v.units() + offset.offset);
        apply.set(true);

        qDebug() << "32 value" << coord.get();
        ok = write(coord, apply);
    }
    void visit(const Coords::DigitOffset& offset) override
    {
        IntTag coord{a.coord};
        BoolTag apply{a.apply};
        coord.set(v.to_digits(offset.per_mm) + offset.offset);
        apply.set(true);

        qDebug() << "16 value" << coord.get();
        ok = write(coord, apply);
    }
    void visit(const Coords::BrokenOffset& offset) override
    {
        DIntTag coord{a.coord};
        BoolTag apply{a.apply};
        coord.set(v.to_digits(offset.per_mm) + offset.offset);
        apply.set(true);

        qDebug() << "32 broken value" << coord.get();
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

    // debug
    {
        qDebug() << "offsets size == " << offsets.size();
    }
    auto o = offsets.find(id);
    if (o != offsets.end())
    {
        //if (false)
        {
            const auto& a = coord_map[id];
            ok = OffsetWriter{a, value, this}.write(o->second);
        }
    }
    offsetList->clear(offsets);
    return ok;
}

bool Alpha::applyCoordinates(const Coords::Coordinates &coords, bool verticalMode)
{
    auto offsets = offsetList->offsets();
    bool ok = true;
    for(const auto& o: offsets)
    {
        auto coordValue = coords.byId(o.first);
        //if (false)
        {
            const auto& a = coord_map[o.first];
            ok = OffsetWriter{a, coordValue, this}.write(o.second);
            if (!ok)
            {
                break;
            }
        }
    }
    offsetList->clear(offsets);
    if (ok)
    {
        BoolTag disable_rotator{Const::disableRotator};
        disable_rotator.set(verticalMode);
        ok = writeTag(disable_rotator);
    }
    return ok;
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

    // P2 / PKA350
    {
        coord_map[POS_ID_P2_PKA350_LEFT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 190},
            .apply = {.area = area, .db = 0, .byte = 36, .bit = 0}};
        coord_map[POS_ID_P2_PKA350_RIGHT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 194},
            .apply = {.area = area, .db = 0, .byte = 36, .bit = 1}};
        coord_map[POS_ID_P2_PKA350_TOP_LEFT] = {
            .coord = {.area = area, .db = 0, .byte = 182},
            .apply = {.area = area, .db = 0, .byte = 35, .bit = 1}};
        coord_map[POS_ID_P2_PKA350_TOP_RIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 186},
            .apply = {.area = area, .db = 0, .byte = 35, .bit = 2}};
        coord_map[POS_ID_P2_PKA350_BOTTOM_LEFT] = {
            .coord = {.area = area, .db = 0, .byte = 174},
            .apply = {.area = area, .db = 0, .byte = 34, .bit = 7}};
        coord_map[POS_ID_P2_PKA350_BOTTOM_RIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 178},
            .apply = {.area = area, .db = 0, .byte = 35, .bit = 0}};
        coord_map[POS_ID_P2_PKA350_LEFT_ROLLER] = {
            .coord = {.area = area, .db = 0, .byte = 198},
            .apply = {.area = area, .db = 0, .byte = 36, .bit = 5}};
        coord_map[POS_ID_P2_PKA350_RIGHT_ROLLER] = {
            .coord = {.area = area, .db = 0, .byte = 202},
            .apply = {.area = area, .db = 0, .byte = 36, .bit = 6}};
        coord_map[POS_ID_P2_PKA350_LEFT_SAW] = {
            .coord = {.area = area, .db = 0, .byte = 206},
            .apply = {.area = area, .db = 0, .byte = 37, .bit = 2}};
        coord_map[POS_ID_P2_PKA350_RIGHT_SAW] = {
            .coord = {.area = area, .db = 0, .byte = 210},
            .apply = {.area = area, .db = 0, .byte = 37, .bit = 3}};
        coord_map[POS_ID_P2_PKA350_ROLLERS_INP_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 718},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 1}};
        coord_map[POS_ID_P2_PKA350_ROLLERS_OUT_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 720},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 2}};
        coord_map[POS_ID_P2_PKA350_ROTATOR_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 722},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 3}};
        coord_map[POS_ID_P2_PKA350_LEFT_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 730},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 7}};
        coord_map[POS_ID_P2_PKA350_RIGHT_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 734},
            .apply = {.area = area, .db = 0, .byte = 47, .bit = 0}};
        coord_map[POS_ID_P2_PKA350_PRESS_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 738},
            .apply = {.area = area, .db = 0, .byte = 47, .bit = 1}};
    }

    // P3 / PA300
    {
        coord_map[POS_ID_P3_PA300_LEFT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 214},
            .apply = {.area = area, .db = 0, .byte = 37, .bit = 7}};
        coord_map[POS_ID_P3_PA300_RIGHT_BLOCK] = {
            .coord = {.area = area, .db = 0, .byte = 218},
            .apply = {.area = area, .db = 0, .byte = 38, .bit = 0}};
        coord_map[POS_ID_P3_PA300_TOP_LEFT] = {
            .coord = {.area = area, .db = 0, .byte = 230},
            .apply = {.area = area, .db = 0, .byte = 38, .bit = 6}};
        coord_map[POS_ID_P3_PA300_TOP_RIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 234},
            .apply = {.area = area, .db = 0, .byte = 38, .bit = 7}};
        coord_map[POS_ID_P3_PA300_BOTTOM_LEFT] = {
            .coord = {.area = area, .db = 0, .byte = 222},
            .apply = {.area = area, .db = 0, .byte = 38, .bit = 4}};
        coord_map[POS_ID_P3_PA300_BOTTOM_RIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 226},
            .apply = {.area = area, .db = 0, .byte = 38, .bit = 5}};
        coord_map[POS_ID_P3_PA300_ROLLERS_INP_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 724},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 4}};
    }

    // DWS
    {
        coord_map[POS_ID_DWS350_PRESS1_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 238},
            .apply = {.area = area, .db = 0, .byte = 29, .bit = 0}};
        coord_map[POS_ID_DWS350_PRESS2_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 242},
            .apply = {.area = area, .db = 0, .byte = 29, .bit = 1}};
        coord_map[POS_ID_DWS350_PRESS3_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 246},
            .apply = {.area = area, .db = 0, .byte = 29, .bit = 2}};
        coord_map[POS_ID_DWS350_INP_ROLLERS_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 726},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 5}};
        coord_map[POS_ID_DWS350_OUT_ROLLERS_WIDTH] = {
            .coord = {.area = area, .db = 0, .byte = 728},
            .apply = {.area = area, .db = 0, .byte = 46, .bit = 6}};
        coord_map[POS_ID_DWS350_AXIS_HEIGHT] = {
            .coord = {.area = area, .db = 0, .byte = 742},
            .apply = {.area = area, .db = 0, .byte = 47, .bit = 2}};
    }
}

} // namespace Schematics::Service
