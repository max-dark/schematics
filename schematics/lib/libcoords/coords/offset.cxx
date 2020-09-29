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
}// namespace Coords
