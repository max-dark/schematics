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

AlphaSupport::~AlphaSupport() = default;

} // namespace Schematics::Service
