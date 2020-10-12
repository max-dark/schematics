#include <services/alpha.hxx>
#include <services/machine.hxx>

#include <QDebug>

namespace Schematics::Service
{
namespace Const
{
static const BitAddress axisStartInit{ .area = Tag::Area::MEMORY, .db = 0, .byte =30, .bit = 0 };
static const BitAddress axisInitDone{ .area = Tag::Area::MEMORY, .db = 0, .byte = 30, .bit = 1 };

/// %M2000.3 - статус КДО:
/// разрешить подачу в цех, если КДО запущено
static const BitAddress kdoIsRunning{ .area = Tag::Area::MEMORY, .db = 0, .byte = 2000, .bit = 3 };
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
    initMemoryMap();
}

bool Alpha::axisInitIsDone() const
{
    return readCachedBit(Const::axisInitDone);
}

bool Alpha::axisStartInit() const
{
    BoolTag start{Const::axisStartInit};
    start.set(true);
    return writeTag(start);
}

bool Alpha::axisResetInit() const
{

    BoolTag reset_start{Const::axisStartInit};
    BoolTag reset_done{Const::axisInitDone};
    reset_start.set(false);
    reset_done.set(false);
    return writeTag(reset_start) && writeTag(reset_done);
}

bool Alpha::allowFeederToWork(bool allow)
{
    BoolTag allow_flag{Const::kdoIsRunning};
    allow_flag.set(allow);

    return writeTag(allow_flag);
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

bool Alpha::applyCoordinates(const Coords::Coordinates &coords)
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
        disable_rotator.set(coords.isVerticalMode());
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

void Alpha::initMemoryMap()
{
    // Дискретные входы и выходы по шкафам
    // Большинство сигналов не интересно
    // Поэтому регистрация прописана, но закоментирована

    // Щитовая операторской
    {
        // SS01 / питание и пускатели
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 0}, 8);
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 0}, 4);
        // SS20
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 10}, 3);
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 10}, 1);
        // SS40
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 16}, 6);
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 16}, 2);
        // SS41
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 24}, 3);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 256}, 4 * sizeof(Tag::Word)); // измерение тока
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 24}, 1);
        // SS42
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 30}, 3);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 264}, 4 * sizeof(Tag::Word)); // измерение тока
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 30}, 1);
        // SS44
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 38}, 3);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 272}, 4 * sizeof(Tag::Word)); // измерение тока
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 38}, 1);
        // SS52
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 42}, 5);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 280}, 6 * sizeof(Tag::Word)); // измерение тока
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 42}, 2);
    }

    // Пульты операторской
    {
        // добавлено просто чтобы было)

        // BP01 / пульт слева
        // registerCacheArea({.area = Tag::Area::INPUT , .byte = 120}, 11);
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 120}, 3);
        // BP02 пульт справа
        // registerCacheArea({.area = Tag::Area::INPUT , .byte = 140}, 10);
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 140}, 4);
    }

    // Щиты на линии
    {
        // KK10 и KK11 не установлены, пропустим их

        // KK20 / FBS1
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 72}, 6);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 412}, 1 * sizeof(Tag::DWord)); // энкодер
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 308}, 6 * sizeof(Tag::Word)); // положение
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 72}, 7);

        // KK30 / FBS2
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 80}, 4);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 324}, 4 * sizeof(Tag::Word)); // положение
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 80}, 7);

        // KK40 / PA350+PKA350
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 88}, 2);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 424}, 3 * sizeof(Tag::DWord)); // положение
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 336}, 4 * sizeof(Tag::Word)); // положение
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 88}, 8);

        // KK50 / PA300+DWS350
        registerCacheArea({.area = Tag::Area::INPUT, .byte = 96}, 4);
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 448}, 1 * sizeof(Tag::DWord)); // положение
        // registerCacheArea({.area = Tag::Area::INPUT, .byte = 348}, 8 * sizeof(Tag::Word)); // положение
        // registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 96}, 5);
    }

    // Управляющие флаги
    {
        // аварии
        registerCacheArea({.area = Tag::Area::MEMORY, .byte = 100}, 16);
        // запуск двигателей
        registerCacheArea({.area = Tag::Area::MEMORY, .byte = 10}, 16);
        // статус инициализации "осей"
        registerCacheArea({.area = Tag::Area::MEMORY, .byte = 30}, 32);
    }
}

} // namespace Schematics::Service
