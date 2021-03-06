#ifndef SCHEMATICS_OFFSET_HXX
#define SCHEMATICS_OFFSET_HXX

#include <schema/units.hxx>
#include <coords/positionid.hxx>

#include <cstdint>
#include <map>

namespace Coords
{
    struct Offset;
    struct UnitOffset;
    struct DigitOffset;
    struct BrokenOffset;

    using OffsetList = std::map<PositionId, Offset*>;

    /**
     * есть три типа "смещений"
     * unit - для координат с соотношением 1:1
     * digit - для координат, имеющих другой коэфициэнт
     * broken - для "сломанных" unit-ов
     */
    enum OffsetType
    {
        UNIT_OFFSET, DIGIT_OFFSET, BROKEN_OFFSET
    };

    struct OffsetVisitor
    {
        virtual void visit(const UnitOffset& offset) = 0;
        virtual void visit(const DigitOffset& offset) = 0;
        virtual void visit(const BrokenOffset& offset) = 0;

    protected:
        virtual ~OffsetVisitor();
    };

    struct OffsetRepository: private OffsetVisitor
    {
        bool try_save(PositionId id, const Offset& offset);
        OffsetList offsets();
        void clear(OffsetList &list);
        Offset* createUnit(int32_t offset);
        Offset* createDigit(int32_t offset, double per_mm);
        Offset* createBroken(int32_t offset, double per_mm);
        Offset* createOffset(int type_id, int32_t offset, double per_mm);
    protected:
        virtual bool try_save(PositionId id, OffsetType type, int32_t offset, double per_mm) = 0;
        virtual bool try_load(OffsetList &list) = 0;

    private:
        void visit(const UnitOffset& offset) override;
        void visit(const DigitOffset& offset) override;
        void visit(const BrokenOffset& offset) override;
        PositionId current_id;
        bool current_status;
    };

    struct Offset {
        virtual void accept(OffsetVisitor& visitor) const = 0;
        virtual ~Offset();
    };

    struct UnitOffset: public Offset
    {
        void accept(OffsetVisitor& visitor) const override;
        int32_t  offset = 0;
    };

    struct DigitOffset: public Offset
    {
        void accept(OffsetVisitor& visitor) const override;
        int32_t offset = 0;
        double per_mm = 100.0;
    };

    struct BrokenOffset: public Offset
    {
        void accept(OffsetVisitor& visitor) const override;
        int32_t offset = 0;
        double per_mm = 100.0;
    };

} // namespace Coords

#endif //SCHEMATICS_OFFSET_HXX
