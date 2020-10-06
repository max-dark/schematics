#ifndef SCHEMATICS_TAGS_HXX
#define SCHEMATICS_TAGS_HXX

#include <cstdint>
#include <string>
#include <unordered_map>

#include <coords/positionid.hxx>

namespace Schematics::Service
{

struct Tag
{
    using Byte = std::uint8_t;
    using Word = std::uint16_t;
    using DWord = std::uint32_t;
    using Int = std::int16_t;
    using DInt = std::int32_t;
    enum class Area
    {
        INPUT, OUTPUT, MEMORY, DATA
    };
    enum class Type
    {
        BIT, BYTE, WORD, DWORD, INT, DINT
    };
    int areaCode() const;
    int typeCode() const;
    int address() const;
    int blockNo() const;
    int itemCount() const;
    virtual const void* pointer() const = 0;
    virtual void* pointer() = 0;

    static int areaCode(Area area);
    static int typeCode(Type type);
protected:
    virtual ~Tag();
    Area area = Area::MEMORY;
    Type type = Type::BIT;
    int start = 0;
    int count = 1;
    int block = 0;
};


struct TagAddress
{
    Tag::Area area = Tag::Area::MEMORY;
    int db = 0;
    int byte = 0;
    static TagAddress from_string(const std::string& tag, bool& ok);
    [[nodiscard]] std::string to_string() const;
};

struct BitAddress
{
    Tag::Area area = Tag::Area::MEMORY;
    int db = 0;
    int byte = 0;
    int bit = 0;
    static BitAddress from_string(const std::string& tag, bool& ok);
    [[nodiscard]] std::string to_string() const;
};

struct CoordAddress
{
    TagAddress coord;
    BitAddress apply;
};

using CoordMap = std::unordered_map<Coords::PositionId, CoordAddress>;

struct BoolTag: public Tag
{
    explicit BoolTag(const BitAddress& address);
    BoolTag(Area area_id,
            int byte_no, int bit_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(bool v) { value = v; }
    bool get() const { return value; }
    void invert() { set(!get()); }
private:
    bool value;
};


struct ByteTag: public Tag
{
    explicit ByteTag(const TagAddress& address);
    ByteTag(Area area_id,
            int byte_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(Byte v) { value = v; }
    Byte get() const { return value; }
private:
    Byte value;
};


struct IntTag: public Tag
{
    explicit IntTag(const TagAddress &address);
    IntTag(Area area_id,
           int byte_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(Int v);
    Int get() const;
private:
    Int value;
};

struct DIntTag: public Tag
{
    explicit DIntTag(const TagAddress &address);
    DIntTag(Area area_id,
            int byte_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(DInt v);
    DInt get() const;
private:
    DInt value;
};

} // namespace Schematics::Service

#endif //SCHEMATICS_TAGS_HXX
