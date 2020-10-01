#include "tags.hxx"

#include <s7_types.h>

#include <cstdint>
#include <type_traits>
#ifdef WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

#include <sstream>

namespace Schematics::Service
{

namespace tool
{
template<class T>
T to_plc(T v)
{
    static_assert(std::is_integral_v<T>, "T must be a number");
    static_assert(sizeof (T) == 2 || sizeof (T) == 4, "sizeof T");
    void * ptr = &v;

    if constexpr(sizeof(T) == 2)
    {
        auto &ref = *static_cast<uint16_t*>(ptr);
        ref = htons(ref);
    }
    else if constexpr(sizeof(T) == 4)
    {
        auto &ref = *static_cast<uint32_t*>(ptr);
        ref = htonl(ref);
    }

    return v;
}

template<class T>
T from_plc(T v)
{
    static_assert(std::is_integral_v<T>, "T must be a number");
    static_assert(sizeof (T) == 2 || sizeof (T) == 4, "sizeof T");
    void * ptr = &v;

    if constexpr(sizeof(T) == 2)
    {
        auto &ref = *static_cast<uint16_t*>(ptr);
        ref = ntohs(ref);
    }
    else if constexpr(sizeof(T) == 4)
    {
        auto &ref = *static_cast<uint32_t*>(ptr);
        ref = ntohl(ref);
    }

    return v;
}

char to_char(Tag::Area area)
{
    switch (area) {
    case Tag::Area::DATA  : return 'D';
    case Tag::Area::INPUT : return 'I';
    case Tag::Area::MEMORY: return 'M';
    case Tag::Area::OUTPUT: return 'Q';
    }
    return 0;
}
Tag::Area from_char(char code, bool& ok)
{
    auto area = Tag::Area::MEMORY;
    switch (code)
    {
    case 'D': ok = true; return Tag::Area::DATA;
    case 'I': ok = true; return Tag::Area::INPUT;
    case 'M': ok = true; return Tag::Area::MEMORY;
    case 'Q': ok = true; return Tag::Area::OUTPUT;
    default: ok = false; break;
    }
    return area;
}
} // tool


int Tag::areaCode() const
{
    switch (area) {
    case Area::DATA  : return S7AreaDB;
    case Area::INPUT : return S7AreaPE;
    case Area::MEMORY: return S7AreaMK;
    case Area::OUTPUT: return S7AreaPA;
    }
    return 0;
}

int Tag::typeCode() const
{
    switch (type) {
    case Type::BIT: return S7WLBit;
    case Type::BYTE: return S7WLByte;
    case Type::INT: return S7WLInt;
    case Type::WORD: return S7WLWord;
    case Type::DINT: return S7WLDInt;
    case Type::DWORD: return S7WLDWord;
    }
    return 0;
}

int Tag::address() const
{
    return start;
}

int Tag::blockNo() const
{
    return block;
}

int Tag::itemCount() const
{
    return count;
}

Tag::~Tag() = default;

BoolTag::BoolTag(Tag::Area area_id,
                 int byte_no, int bit_no, int block_no)
        : Tag{}
{
    area = area_id;
    type = Type::BIT;
    start = byte_no << 3 | bit_no;
    block = block_no;
    value = false;
}

BoolTag::BoolTag(const BitAddress &address)
    :BoolTag(address.area, address.byte, address.bit, address.db)
{}

ByteTag::ByteTag(Tag::Area area_id,
                 int byte_no, int block_no)
        : Tag{}
{
    area = area_id;
    type = Type::BYTE;
    start = byte_no << 3;
    block = block_no;
    value = 0;
}

ByteTag::ByteTag(const TagAddress &address)
    : ByteTag(address.area, address.byte, address.db)
{}

IntTag::IntTag(Tag::Area area_id, int byte_no, int block_no)
        : Tag{}
{
    area = area_id;
    type = Type::INT;
    start = byte_no << 3;
    block = block_no;
    value = 0;
}

IntTag::IntTag(const TagAddress &address)
        :IntTag(address.area, address.byte, address.db)
{}

void IntTag::set(Tag::Int v)
{
    value = tool::to_plc(v);
}

Tag::Int IntTag::get() const
{
    return tool::from_plc(value);
}

DIntTag::DIntTag(Tag::Area area_id, int byte_no, int block_no)
        : Tag{}
{
    area = area_id;
    type = Type::DINT;
    start = byte_no << 3;
    block = block_no;
    value = 0;
}

DIntTag::DIntTag(const TagAddress &address)
        : DIntTag(address.area, address.byte, address.db)
{}

void DIntTag::set(Tag::DInt v)
{
    value = tool::to_plc(v);
}

Tag::DInt DIntTag::get() const
{
    return tool::from_plc(value);
}


TagAddress TagAddress::from_string(const std::string &tag, bool& ok)
{
    TagAddress addr;
    char code, any_char;
    std::istringstream parser{tag};
    parser >> code >> addr.db >> any_char >> addr.byte;
    addr.area = tool::from_char(code, ok);
    ok = ok && parser.good();

    return addr;
}

std::string TagAddress::to_string() const
{
    std::ostringstream out;
    out << tool::to_char(area) << db << ':' << byte;
    return out.str();
}

BitAddress BitAddress::from_string(const std::string &tag, bool &ok)
{
    BitAddress addr;
    char code, any_char;
    std::istringstream parser{tag};
    parser >> code >> addr.db >> any_char >> addr.byte >> any_char >> addr.bit;
    addr.area = tool::from_char(code, ok);
    ok = ok && parser.good();
    return addr;
}

std::string BitAddress::to_string() const
{
    std::ostringstream out;
    out << tool::to_char(area) << db << ':' << byte << '.' << bit;
    return out.str();
}
} // namespace Schematics::Service
