#include "offset.hxx"

namespace Coords
{

    OffsetVisitor::~OffsetVisitor() = default;

    Offset::~Offset() = default;

    void UnitOffset::accept(OffsetVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void DigitOffset::accept(OffsetVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    bool OffsetRepository::try_save(PositionId id, const Offset &offset) {
        current_id = id;
        offset.accept(*this);
        return current_status;
    }

    void OffsetRepository::visit(const UnitOffset &offset) {
        current_status = try_save(current_id, UNIT_OFFSET, offset.offset, 100.0);
    }

    void OffsetRepository::visit(const DigitOffset &offset) {
        current_status = try_save(current_id, DIGIT_OFFSET, offset.offset, offset.per_mm);
    }

    void OffsetRepository::visit(const BrokenOffset &offset)
    {
        current_status = try_save(current_id, BROKEN_OFFSET, offset.offset, offset.per_mm);
    }

    OffsetList OffsetRepository::offsets() {
        Coords::OffsetList list;
        if (try_load(list))
        {
            return list;
        }
        return {};
    }

    Offset *OffsetRepository::createDigit(int32_t offset, double per_mm) {
        auto o = new DigitOffset{};
        o->offset = offset;
        o->per_mm = per_mm;
        return o;
    }

    Offset *OffsetRepository::createBroken(int32_t offset, double per_mm)
    {
        auto o = new BrokenOffset{};
        o->offset = offset;
        o->per_mm = per_mm;
        return o;
    }

    Offset *OffsetRepository::createUnit(int32_t offset) {
        auto o = new UnitOffset;
        o->offset = offset;
        return o;
    }

    void OffsetRepository::clear(OffsetList &list) {
        for(auto p: list)
        {
            delete p.second;
        }
        list.clear();
    }

Offset *OffsetRepository::createOffset(int type_id, int32_t offset, double per_mm)
{
    auto type =  (type_id >=0 && type_id <=2)
                 ? OffsetType(type_id)
                 : BROKEN_OFFSET;

    switch (type) {
    case DIGIT_OFFSET: return createDigit(offset, per_mm);
    case UNIT_OFFSET: return createUnit(offset);
    case BROKEN_OFFSET: return createBroken(offset, per_mm);
    }
    return nullptr;
}

void BrokenOffset::accept(OffsetVisitor &visitor) const
    {
        visitor.visit(*this);
    }

}// namespace Coords
