#include "machine.hxx"

#include <s7_micro_client.h>

#include <cstdint>
#include <type_traits>
#ifdef WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

char * ErrCliText(int Error, char* Result, int TextLen);

namespace Schematics::Service
{

namespace tool {
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

    if constexpr(sizeof(T) == 4)
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

    if constexpr(sizeof(T) == 4)
    {
        auto &ref = *static_cast<uint32_t*>(ptr);
        ref = ntohl(ref);
    }

    return v;
}
} // tool

struct Machine::Connection: protected TSnap7MicroClient
{
    bool connect(const QByteArray& address, int rack, int slot)
    {
        return ok(ConnectTo(address.data(), rack, slot));
    }
    bool connected() const noexcept
    {
        return Connected;
    }
    bool disconnect()
    {
        return ok(Disconnect());
    }

    /**
     * @brief write write data from array[count] of type
     * @param area area id
     * @param block_no data block no
     * @param start start address
     * @param count count items in array
     * @param type type of items in array
     * @param data array
     * @return
     */
    bool write(int area, int block_no, int start,
               int count, int type, const void* data)
    {
        return ok(WriteArea(
            area, block_no, start, count, type, const_cast<void*>(data)
            ));
    }

    /**
     * @brief read read data to array[count] of type
     * @param area area id
     * @param block_no data block no
     * @param start start address
     * @param count count items in array
     * @param type type of items in array
     * @param data array
     * @return
     */
    bool read(int area, int block_no, int start,
              int count, int type, void* data)
    {
        return ok(ReadArea(
            area, block_no, start, count, type, data
            ));
    }
    bool readMany(PS7DataItem data, int count)
    {
        return ok(ReadMultiVars(data, count));
    }
    bool writeMany(const TS7DataItem* data, int count)
    {
        return ok(WriteMultiVars(const_cast<PS7DataItem>(data), count));
    }

    int lastError() const { return last_op; }
    bool hasError() const { return error(); }
    QString errorMessage() const
    {
        QByteArray tmp{512, '\0'};
        return QString{
            ErrCliText(lastError(), tmp.data(), tmp.size())
        };
    }
private:
    bool ok(int status) {
        last_op = status;
        return ok();
    }
    bool error() const { return !ok(); };
    bool ok() const { return 0 == last_op; }
    int last_op = 0;
};

Machine::Machine(QObject *parent)
        : QObject(parent)
        , selfptr{new Connection{}}
{}

Machine::~Machine()
{
    disconnect();
    delete self();
}

bool Machine::connect(const QString &address,
                      int rack, int slot) const
{
    return self()->connect(
        address.toLocal8Bit(),
        rack, slot
        );
}

bool Machine::connected() const
{
    return self()->connected();
}

void Machine::disconnect() const
{
    self()->disconnect();
}

bool Machine::readTag(Tag &tag) const
{
    return self()->read(
        tag.areaCode(),
        tag.blockNo(),
        tag.address(),
        tag.itemCount(),
        tag.typeCode(),
        tag.pointer()
                );
}

bool Machine::writeTag(const Tag &tag) const
{
    return self()->write(
        tag.areaCode(),
        tag.blockNo(),
        tag.address(),
        tag.itemCount(),
        tag.typeCode(),
        tag.pointer()
        );
}

bool Machine::hasError() const
{
    return self()->hasError();
}

int Machine::lastError() const
{
    return self()->lastError();
}

QString Machine::errorMessage() const
{
    return self()->errorMessage();
}

Machine::Connection *Machine::self() const
{
    return selfptr;
}

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
    start = (byte_no << 3) | bit_no;
    block = block_no;
    value = false;
}

ByteTag::ByteTag(Tag::Area area_id,
                 int byte_no, int block_no)
    : Tag{}
{
    area = area_id;
    type = Type::BYTE;
    start = (byte_no << 3);
    block = block_no;
    value = 0;
}

IntTag::IntTag(Tag::Area area_id, int byte_no, int block_no)
    : Tag{}
{
    area = area_id;
    type = Type::INT;
    start = (byte_no << 3);
    block = block_no;
    value = 0;
}

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
    start = (byte_no << 3);
    block = block_no;
    value = 0;
}

void DIntTag::set(Tag::DInt v)
{
    value = tool::to_plc(v);
}

Tag::DInt DIntTag::get() const
{
    return tool::from_plc(value);
}

} // namespace Schematics::Service
