#include "machine.hxx"

#include <s7_micro_client.h>

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

bool Machine::readBlock(const TagAddress &address, size_t size, void *block) const
{
    ByteTag tmp{address};
    return self()->read(
        tmp.areaCode(),
        tmp.blockNo(),
        tmp.address(),
        size,
        tmp.typeCode(),
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

} // namespace Schematics::Service
