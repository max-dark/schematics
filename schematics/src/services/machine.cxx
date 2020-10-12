#include "machine.hxx"

#include <s7_micro_client.h>

#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

char * ErrCliText(int Error, char* Result, int TextLen);

namespace Schematics::Service
{

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

    bool resetConnection()
    {
        ClrError();
        return ok(Reset(true));
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

struct MemoryArea
{
    MemoryArea(const TagAddress &start, std::size_t length) noexcept
            : start(start), length(length)
    {}

    TagAddress start;
    std::size_t length;
};

struct Machine::Memory
{
    static constexpr std::size_t BLOCK_SIZE = 64 * 1024u;
    using Block = std::array<std::uint8_t, BLOCK_SIZE>;
    using BlockMap = std::unordered_map<int, Block>;
    using AreaList = std::vector<MemoryArea>;

    Block inputs{};
    Block outputs{};
    Block flags{};
    BlockMap data{};
    AreaList areaList{};
    const Block &fromAddress(Tag::Area area, int db) const
    {
        switch (area)
        {
        case Tag::Area::INPUT:
            return inputs;
        case Tag::Area::OUTPUT:
            return outputs;
        case Tag::Area::MEMORY:
            return flags;
        case Tag::Area::DATA:
            return data.at(db);
        }
        throw std::invalid_argument("Unknown area in Machine::Memory::pointer");
    }

    Block &fromAddress(Tag::Area area, int db)
    {
        switch (area)
        {
        case Tag::Area::INPUT:
            return inputs;
        case Tag::Area::OUTPUT:
            return outputs;
        case Tag::Area::MEMORY:
            return flags;
        case Tag::Area::DATA:
            return data.at(db);
        }
        throw std::invalid_argument("Unknown area in Machine::Memory::pointer");
    }
    const Block &fromAddress(const TagAddress& address) const
    {
        return fromAddress(address.area, address.db);
    }
    Block &fromAddress(const TagAddress& address)
    {
        return fromAddress(address.area, address.db);
    }
    const Block &fromAddress(const BitAddress& address) const
    {
        return fromAddress(address.area, address.db);
    }
    Block &fromAddress(const BitAddress& address)
    {
        return fromAddress(address.area, address.db);
    }
    void * pointer(const TagAddress& address)
    {
        return fromAddress(address).data()+ address.byte;
    }
};

Machine::Machine(QObject *parent)
        : QObject(parent)
        , selfptr{new Connection{}}
        , cache{new Memory{}}
{}

Machine::~Machine()
{
    disconnect();
    delete self();
    delete cache;
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

bool Machine::reconnect() const
{
    return self()->resetConnection();
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

bool Machine::readBlock(const TagAddress &address, size_t size, void *block) const
{
    return self()->read(
        Tag::areaCode(address.area),
        address.db,
        address.byte,
        size,
        Tag::typeCode(Tag::Type::BYTE),
        block
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

void Machine::registerCacheArea(const TagAddress &start, std::size_t length) const
{
    if (Tag::Area::DATA == start.area)
    {
        cache->data.try_emplace(start.db, Memory::Block{});
    }
    cache->areaList.emplace_back(start, length);
}

bool Machine::updateCache() const
{
    if (!connected()) return false;

    if constexpr ((false))
    {
        for (const auto& area: cache->areaList)
        {
            auto ok = readBlock(
                area.start, area.length,
                cache->pointer(area.start)
                );
            if (!ok)
            {
                return false;
            }
        }
    }
    else
    {
        auto cnt = cache->areaList.size();
        TS7DataItem data[MaxVars];
        PS7DataItem ptr = data;

        for (const auto& area: cache->areaList)
        {
            ptr->Area = Tag::areaCode(area.start.area);
            ptr->WordLen = Tag::typeCode(Tag::Type::BYTE);
            ptr->Amount = area.length;
            ptr->Start = area.start.byte;
            ptr->DBNumber = area.start.db;
            ptr->pdata = cache->pointer(area.start);
            ptr->Result = 0;
            ++ptr;
        }

        auto ok = self()->readMany(data, cnt);
        auto idx = 0u;
        while (ok && (idx < cnt))
        {
            ok = 0 == data[idx].Result;
            ++idx;
        }

        return ok;
    }
    return true;
}

Tag::Byte Machine::readCachedByte(const TagAddress &address) const
{
    return cache->fromAddress(address).at(address.byte);
}

bool Machine::readCachedBit(const BitAddress &address) const
{
    auto byte = cache->fromAddress(address).at(address.byte);
    return 0 != (byte & (1u << static_cast<unsigned>(address.bit)));
}

} // namespace Schematics::Service
