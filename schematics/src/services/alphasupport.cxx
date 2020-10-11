#include "alphasupport.hxx"

namespace Schematics::Service
{

AlphaSupport::AlphaSupport(QObject *parent)
        : Machine(parent)
{
    initMemoryMap();
}

void AlphaSupport::initMemoryMap()
{
    // CPU 313C IO
    registerCacheArea({.area = Tag::Area::INPUT , .byte = 124}, 3);
    registerCacheArea({.area = Tag::Area::OUTPUT, .byte = 124}, 2);
    // CPU 313C retentive memory:
    // конфигурация, флаги состояния
    // %MB10 занят под "Clock Memory"
    registerCacheArea({.area = Tag::Area::MEMORY, .byte = 0}, 16);
    // DI16xDC24V
    registerCacheArea({.area = Tag::Area::INPUT , .byte = 0}, 2);
}

AlphaSupport::StateMap AlphaSupport::getMotorsState() const
{
    //    сигналы ПП/ПЧ: ОК(NO, замыкаются при пуске)
    const BitAddress motor_on[] = {
            //        #1 дальний уличный - I 1.1
            {.area = Tag::Area::INPUT, .db = 0, .byte = 1, .bit = 1},
            //        #2 ближний уличный - I 1.3
            {.area = Tag::Area::INPUT, .db = 0, .byte = 1, .bit = 3},
            //        #3 ФБС 1/2         - I 0.2
            {.area = Tag::Area::INPUT, .db = 0, .byte = 0, .bit = 2},
            //        #4 профиляторы 1/2 - I 0.6
            {.area = Tag::Area::INPUT, .db = 0, .byte = 0, .bit = 6},
            //        #5 многопил        - I 1.4
            {.area = Tag::Area::INPUT, .db = 0, .byte = 1, .bit = 4},
    };
    StateMap state;
    for (auto idx = 0; idx < std::size(motor_on); ++idx)
    {
        state[idx] = readCachedBit(motor_on[idx]);
    }
    return state;
}

static inline bool no_error(bool state)
{
    return !state;
}

AlphaSupport::StateMap AlphaSupport::getAlarmsState() const
{
    //    сигналы "Авария ПП/ПЧ"
    const BitAddress motor_fault[] = {
            //        #1 дальний уличный - M 0.7
            {.area = Tag::Area::MEMORY, .db = 0, .byte = 0, .bit = 7},
            //        #2 ближний уличный - M 1.1
            {.area = Tag::Area::MEMORY, .db = 0, .byte = 1, .bit = 1},
            //        #3 ФБС 1/2         - M 0.0
            {.area = Tag::Area::MEMORY, .db = 0, .byte = 0, .bit = 0},
            //        #4 профиляторы 1/2 - M 0.4
            {.area = Tag::Area::MEMORY, .db = 0, .byte = 0, .bit = 4},
            //        #5 многопил        - M 1.2
            {.area = Tag::Area::MEMORY, .db = 0, .byte = 1, .bit = 2}
    };
    StateMap state;
    for (auto idx = 0; idx < std::size(motor_fault); ++idx)
    {
        // invert logic
        state[idx] = no_error(readCachedBit(motor_fault[idx]));
    }
    return state;
}

AlphaSupport::~AlphaSupport() = default;

} // namespace Schematics::Service
